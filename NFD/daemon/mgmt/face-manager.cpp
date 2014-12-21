/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014,  Regents of the University of California,
 *                      Arizona Board of Regents,
 *                      Colorado State University,
 *                      University Pierre & Marie Curie, Sorbonne University,
 *                      Washington University in St. Louis,
 *                      Beijing Institute of Technology,
 *                      The University of Memphis
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "face-manager.hpp"

#include "core/logger.hpp"
#include "core/config-file.hpp"
#include "face/protocol-factory.hpp"
#include "fw/face-table.hpp"

#include <ndn-cxx/management/nfd-face-event-notification.hpp>
#include <ndn-cxx/management/nfd-face-query-filter.hpp>

namespace nfd {

NFD_LOG_INIT("FaceManager");

const Name FaceManager::COMMAND_PREFIX("/localhost/nfd/faces");

const size_t FaceManager::COMMAND_UNSIGNED_NCOMPS =
  FaceManager::COMMAND_PREFIX.size() +
  1 + // verb
  1;  // verb parameters

const size_t FaceManager::COMMAND_SIGNED_NCOMPS =
  FaceManager::COMMAND_UNSIGNED_NCOMPS +
  4; // (timestamp, nonce, signed info tlv, signature tlv)

const FaceManager::SignedVerbAndProcessor FaceManager::SIGNED_COMMAND_VERBS[] =
  {
    SignedVerbAndProcessor(
                           Name::Component("create"),
                           &FaceManager::createFace
                           ),

    SignedVerbAndProcessor(
                           Name::Component("destroy"),
                           &FaceManager::destroyFace
                           ),

    SignedVerbAndProcessor(
                           Name::Component("enable-local-control"),
                           &FaceManager::enableLocalControl
                           ),

    SignedVerbAndProcessor(
                           Name::Component("disable-local-control"),
                           &FaceManager::disableLocalControl
                           ),
  };

const FaceManager::UnsignedVerbAndProcessor FaceManager::UNSIGNED_COMMAND_VERBS[] =
  {
    UnsignedVerbAndProcessor(
                             Name::Component("list"),
                             &FaceManager::listFaces
                             ),

    UnsignedVerbAndProcessor(
                             Name::Component("events"),
                             &FaceManager::ignoreUnsignedVerb
                             ),

    UnsignedVerbAndProcessor(
                             Name::Component("channels"),
                             &FaceManager::listChannels
                             ),

    UnsignedVerbAndProcessor(
                             Name::Component("query"),
                             &FaceManager::listQueriedFaces
                             ),
  };

const Name FaceManager::FACES_LIST_DATASET_PREFIX("/localhost/nfd/faces/list");
const size_t FaceManager::FACES_LIST_DATASET_NCOMPS = FACES_LIST_DATASET_PREFIX.size();

const Name FaceManager::FACE_EVENTS_PREFIX("/localhost/nfd/faces/events");

const Name FaceManager::CHANNELS_LIST_DATASET_PREFIX("/localhost/nfd/faces/channels");
const size_t FaceManager::CHANNELS_LIST_DATASET_NCOMPS = CHANNELS_LIST_DATASET_PREFIX.size();

const Name FaceManager::FACES_QUERY_DATASET_PREFIX("/localhost/nfd/faces/query");
const size_t FaceManager::FACES_QUERY_DATASET_NCOMPS = FACES_QUERY_DATASET_PREFIX.size() + 1;

FaceManager::FaceManager(FaceTable& faceTable,
                         shared_ptr<InternalFace> face,
                         ndn::KeyChain& keyChain)
  : ManagerBase(face, FACE_MANAGER_PRIVILEGE, keyChain)
  , m_faceTable(faceTable)
  , m_faceAddConn(m_faceTable.onAdd.connect(bind(&FaceManager::onAddFace, this, _1)))
  , m_faceRemoveConn(m_faceTable.onRemove.connect(bind(&FaceManager::onRemoveFace, this, _1)))
  , m_faceStatusPublisher(m_faceTable, *m_face, FACES_LIST_DATASET_PREFIX, keyChain)
  , m_channelStatusPublisher(m_factories, *m_face, CHANNELS_LIST_DATASET_PREFIX, keyChain)
  , m_notificationStream(*m_face, FACE_EVENTS_PREFIX, keyChain)
  , m_signedVerbDispatch(SIGNED_COMMAND_VERBS,
                         SIGNED_COMMAND_VERBS +
                         (sizeof(SIGNED_COMMAND_VERBS) / sizeof(SignedVerbAndProcessor)))
  , m_unsignedVerbDispatch(UNSIGNED_COMMAND_VERBS,
                           UNSIGNED_COMMAND_VERBS +
                           (sizeof(UNSIGNED_COMMAND_VERBS) / sizeof(UnsignedVerbAndProcessor)))

{
  face->setInterestFilter("/localhost/nfd/faces",
                          bind(&FaceManager::onFaceRequest, this, _2));
}

FaceManager::~FaceManager()
{

}

void
FaceManager::setConfigFile(ConfigFile& configFile)
{
  configFile.addSectionHandler("face_system",
                               bind(&FaceManager::onConfig, this, _1, _2, _3));
}


void
FaceManager::onConfig(const ConfigSection& configSection,
                      bool isDryRun,
                      const std::string& filename)
{
  throw Error("Not supported");
}

void
FaceManager::onFaceRequest(const Interest& request)
{
  const Name& command = request.getName();
  const size_t commandNComps = command.size();

  if (commandNComps <= COMMAND_PREFIX.size())
    {
      // command is too short to have a verb
      NFD_LOG_DEBUG("command result: malformed");
      sendResponse(command, 400, "Malformed command");
      return;
    }

  const Name::Component& verb = command.at(COMMAND_PREFIX.size());

  const auto unsignedVerbProcessor = m_unsignedVerbDispatch.find(verb);
  if (unsignedVerbProcessor != m_unsignedVerbDispatch.end())
    {
      NFD_LOG_DEBUG("command result: processing verb: " << verb);
      (unsignedVerbProcessor->second)(this, request);
    }
  else if (COMMAND_UNSIGNED_NCOMPS <= commandNComps &&
           commandNComps < COMMAND_SIGNED_NCOMPS)
    {
      NFD_LOG_DEBUG("command result: unsigned verb: " << command);
      sendResponse(command, 401, "Signature required");
    }
  else if (commandNComps < COMMAND_SIGNED_NCOMPS ||
           !COMMAND_PREFIX.isPrefixOf(command))
    {
      NFD_LOG_DEBUG("command result: malformed");
      sendResponse(command, 400, "Malformed command");
    }
  else
    {
      validate(request,
               bind(&FaceManager::onValidatedFaceRequest, this, _1),
               bind(&ManagerBase::onCommandValidationFailed, this, _1, _2));
    }
}

void
FaceManager::onValidatedFaceRequest(const shared_ptr<const Interest>& request)
{
  const Name& command = request->getName();
  const Name::Component& verb = command[COMMAND_PREFIX.size()];
  const Name::Component& parameterComponent = command[COMMAND_PREFIX.size() + 1];

  SignedVerbDispatchTable::const_iterator signedVerbProcessor = m_signedVerbDispatch.find(verb);
  if (signedVerbProcessor != m_signedVerbDispatch.end())
    {
      ControlParameters parameters;
      if (!extractParameters(parameterComponent, parameters))
        {
          sendResponse(command, 400, "Malformed command");
          return;
        }

      NFD_LOG_DEBUG("command result: processing verb: " << verb);
      (signedVerbProcessor->second)(this, *request, parameters);
    }
  else
    {
      NFD_LOG_DEBUG("command result: unsupported verb: " << verb);
      sendResponse(command, 501, "Unsupported command");
    }

}

void
FaceManager::addCreatedFaceToForwarder(const shared_ptr<Face>& newFace)
{
  m_faceTable.add(newFace);

  //NFD_LOG_DEBUG("Created face " << newFace->getRemoteUri() << " ID " << newFace->getId());
}

void
FaceManager::onCreated(const Name& requestName,
                       ControlParameters& parameters,
                       const shared_ptr<Face>& newFace)
{
  addCreatedFaceToForwarder(newFace);
  parameters.setFaceId(newFace->getId());
  parameters.setUri(newFace->getRemoteUri().toString());

  sendResponse(requestName, 200, "Success", parameters.wireEncode());
}

void
FaceManager::onConnectFailed(const Name& requestName, const std::string& reason)
{
  NFD_LOG_DEBUG("Failed to create face: " << reason);
  sendResponse(requestName, 408, reason);
}

void
FaceManager::createFace(const Interest& request,
                        ControlParameters& parameters)
{
  const Name& requestName = request.getName();
  ndn::nfd::FaceCreateCommand command;

  if (!validateParameters(command, parameters))
    {
      sendResponse(requestName, 400, "Malformed command");
      NFD_LOG_TRACE("invalid control parameters URI");
      return;
    }

  FaceUri uri;
  if (!uri.parse(parameters.getUri()))
    {
      sendResponse(requestName, 400, "Malformed command");
      NFD_LOG_TRACE("failed to parse URI");
      return;
    }

  if (!uri.isCanonical())
    {
      sendResponse(requestName, 400, "Non-canonical URI");
      NFD_LOG_TRACE("received non-canonical URI");
      return;
    }

  FactoryMap::iterator factory = m_factories.find(uri.getScheme());
  if (factory == m_factories.end())
    {
      sendResponse(requestName, 501, "Unsupported protocol");
      return;
    }

  try
    {
      factory->second->createFace(uri,
                                  bind(&FaceManager::onCreated,
                                       this, requestName, parameters, _1),
                                  bind(&FaceManager::onConnectFailed,
                                       this, requestName, _1));
    }
  catch (const std::runtime_error& error)
    {
      std::string errorMessage = "NFD error: ";
      errorMessage += error.what();

      NFD_LOG_ERROR(errorMessage);
      sendResponse(requestName, 500, errorMessage);
    }
  catch (const std::logic_error& error)
    {
      std::string errorMessage = "NFD error: ";
      errorMessage += error.what();

      NFD_LOG_ERROR(errorMessage);
      sendResponse(requestName, 500, errorMessage);
    }
}


void
FaceManager::destroyFace(const Interest& request,
                         ControlParameters& parameters)
{
  const Name& requestName = request.getName();
  ndn::nfd::FaceDestroyCommand command;

  if (!validateParameters(command, parameters))
    {
      sendResponse(requestName, 400, "Malformed command");
      return;
    }

  shared_ptr<Face> target = m_faceTable.get(parameters.getFaceId());
  if (static_cast<bool>(target))
    {
      target->close();
    }

  sendResponse(requestName, 200, "Success", parameters.wireEncode());

}

void
FaceManager::onAddFace(shared_ptr<Face> face)
{
  ndn::nfd::FaceEventNotification notification;
  notification.setKind(ndn::nfd::FACE_EVENT_CREATED);
  face->copyStatusTo(notification);

  m_notificationStream.postNotification(notification);
}

void
FaceManager::onRemoveFace(shared_ptr<Face> face)
{
  ndn::nfd::FaceEventNotification notification;
  notification.setKind(ndn::nfd::FACE_EVENT_DESTROYED);
  face->copyStatusTo(notification);

  m_notificationStream.postNotification(notification);
}

bool
FaceManager::extractLocalControlParameters(const Interest& request,
                                           ControlParameters& parameters,
                                           ControlCommand& command,
                                           shared_ptr<LocalFace>& outFace,
                                           LocalControlFeature& outFeature)
{
  if (!validateParameters(command, parameters))
    {
      sendResponse(request.getName(), 400, "Malformed command");
      return false;
    }

  shared_ptr<Face> face = m_faceTable.get(request.getIncomingFaceId());

  if (!static_cast<bool>(face))
    {
      NFD_LOG_DEBUG("command result: faceid " << request.getIncomingFaceId() << " not found");
      sendResponse(request.getName(), 410, "Face not found");
      return false;
    }
  else if (!face->isLocal())
    {
      NFD_LOG_DEBUG("command result: cannot enable local control on non-local faceid " <<
                    face->getId());
      sendResponse(request.getName(), 412, "Face is non-local");
      return false;
    }

  outFace = dynamic_pointer_cast<LocalFace>(face);
  outFeature = static_cast<LocalControlFeature>(parameters.getLocalControlFeature());

  return true;
}

void
FaceManager::enableLocalControl(const Interest& request,
                                ControlParameters& parameters)
{
  ndn::nfd::FaceEnableLocalControlCommand command;


  shared_ptr<LocalFace> face;
  LocalControlFeature feature;

  if (extractLocalControlParameters(request, parameters, command, face, feature))
    {
      face->setLocalControlHeaderFeature(feature, true);
      sendResponse(request.getName(), 200, "Success", parameters.wireEncode());
    }
}

void
FaceManager::disableLocalControl(const Interest& request,
                                 ControlParameters& parameters)
{
  ndn::nfd::FaceDisableLocalControlCommand command;
  shared_ptr<LocalFace> face;
  LocalControlFeature feature;

  if (extractLocalControlParameters(request, parameters, command, face, feature))
    {
      face->setLocalControlHeaderFeature(feature, false);
      sendResponse(request.getName(), 200, "Success", parameters.wireEncode());
    }
}

void
FaceManager::listFaces(const Interest& request)
{
  const Name& command = request.getName();
  const size_t commandNComps = command.size();

  if (commandNComps < FACES_LIST_DATASET_NCOMPS ||
      !FACES_LIST_DATASET_PREFIX.isPrefixOf(command))
    {
      NFD_LOG_DEBUG("command result: malformed");
      sendResponse(command, 400, "Malformed command");
      return;
    }

  m_faceStatusPublisher.publish();
}

void
FaceManager::listChannels(const Interest& request)
{
  NFD_LOG_DEBUG("in listChannels");
  const Name& command = request.getName();
  const size_t commandNComps = command.size();

  if (commandNComps < CHANNELS_LIST_DATASET_NCOMPS ||
      !CHANNELS_LIST_DATASET_PREFIX.isPrefixOf(command))
    {
      NFD_LOG_DEBUG("command result: malformed");
      sendResponse(command, 400, "Malformed command");
      return;
    }

  NFD_LOG_DEBUG("publishing");
  m_channelStatusPublisher.publish();
}

void
FaceManager::listQueriedFaces(const Interest& request)
{
  NFD_LOG_DEBUG("in listQueriedFaces");
  const Name& query = request.getName();
  const size_t queryNComps = query.size();

  if (queryNComps < FACES_QUERY_DATASET_NCOMPS ||
      !FACES_QUERY_DATASET_PREFIX.isPrefixOf(query))
    {
      NFD_LOG_DEBUG("query result: malformed");
      sendNack(query);
      return;
    }

  ndn::nfd::FaceQueryFilter faceFilter;
  try
    {
      faceFilter.wireDecode(query[-1].blockFromValue());
    }
  catch (tlv::Error&)
    {
      NFD_LOG_DEBUG("query result: malformed filter");
      sendNack(query);
      return;
    }

  FaceQueryStatusPublisher
    faceQueryStatusPublisher(m_faceTable, *m_face, query, faceFilter, m_keyChain);

  faceQueryStatusPublisher.publish();
}

shared_ptr<ProtocolFactory>
FaceManager::findFactory(const std::string& protocol)
{
  FactoryMap::iterator factory = m_factories.find(protocol);
  if (factory != m_factories.end())
    return factory->second;
  else
    return shared_ptr<ProtocolFactory>();
}

} // namespace nfd
