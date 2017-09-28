/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "dummy-keychain.hpp"

#include <ndn-cxx/util/io.hpp>
#include <ndn-cxx/security/transform/public-key.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace ndn {
namespace security {

static const uint8_t DUMMY_CERT[] =
    "Bv0CqQclCAVkdW1teQgDS0VZCAgtnChyuKo7YAgCTkEICf0AAAFe3zzGfBQJGAEC"
    "GQQANu6AFf0BJjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOJSa1j6"
    "JAzobrmtlUdJi38EWQZOigLykf9psImvIu7pa29Q3apBXENcV1E9687FmfY85Ec3"
    "/onMtN7WG/wiuKiu/9eOr5WslD3VGDgxhesx80ygP0GNsN8FzsMl0lRKduXx3wG4"
    "MCT8CX7uA4n4JbHY+0QaKUEEMRCiXcRAtF+yKfr+GaVeSemg+i/LR+6CSgpOyH0K"
    "ogH9nlNhBn1Hxyc8X+B/nEu6P6NNEKkSnhT1jAbWtT1eL3BPGK/HNY19w9k2Ln6/"
    "OYlhaHfB4m0oR/ePcUguQBwTgYS+40YQYUPivBsSQv3X2/7+gGLhIzA4YppwOooK"
    "RGhbmL8zF2evmJ8CAwEAARZHGwEBHBgHFggFZHVtbXkIA0tFWQgILZwocriqO2D9"
    "AP0m/QD+DzIwMTcxMDAyVDIyMzczNv0A/w8yMDE4MTAwMlQyMjM3MzUX/QEAMBUV"
    "O51BEQwxp646i7IuHkuc1C/LISnOP2+wBFF2Ea1ht9MKjKkyJdmB1/GAOiR3njKd"
    "UgRjBlownL11EwYDxkPY39RR05TmhF6PkpP81Ro/Vzv7rtSi/dxFIZXCiyuKPRUx"
    "E0pZelPWVs3zMfqQ+8rWG89Kqs1vM0dglLBzlX9Lbim71TyLmaJaMmmBKv8+eQ22"
    "CN71sRZOovl1kKcTHpOm61nD3C1n9GRflFtaMAXE/XU4zMJVzBv6XwQl6PCIc9H2"
    "vjLa28ruVjhMGxqCGhziTC2eR56SUixrnEcbOKT0R+8+0AFnZIjdYglOZPcVwTVB"
    "G6OxECJOuSoREcd1Ww==;";

static const uint8_t DUMMY_SIGNATURE[] =
    {0x17, 0xfd, 0x01, 0x00, 0x93, 0x15, 0x09, 0x49, 0x79, 0x9e, 0xb7, 0x9c, 0xd3, 0xc1, 0xbf, 0x61,
     0x89, 0xd5, 0xd9, 0xca, 0xf2, 0xb0, 0x14, 0xae, 0x72, 0x7c, 0x1f, 0x8f, 0xf5, 0xb1, 0x70, 0xd6,
     0x9b, 0x8f, 0xf8, 0xd7, 0x2d, 0xbc, 0x92, 0x6f, 0x7d, 0x77, 0x96, 0x46, 0xea, 0xd4, 0x7d, 0x90,
     0xbc, 0x7a, 0xeb, 0xe2, 0x03, 0x93, 0xb1, 0xd2, 0x62, 0xec, 0x9d, 0xff, 0x9c, 0x9c, 0x2a, 0x14,
     0x7d, 0x23, 0xca, 0x29, 0x3d, 0x15, 0x1a, 0x40, 0x42, 0x2c, 0x59, 0x33, 0x8a, 0xf7, 0xc0, 0x6b,
     0xc4, 0x9c, 0xf3, 0xc4, 0x99, 0xa4, 0x1a, 0x60, 0xf5, 0x28, 0x7d, 0x4c, 0xef, 0x43, 0x7d, 0xbd,
     0x7d, 0x00, 0x51, 0xee, 0x41, 0xf5, 0x25, 0x80, 0xce, 0xe6, 0x64, 0x4f, 0x75, 0x54, 0xf3, 0xb2,
     0x99, 0x9a, 0x0f, 0x93, 0x9a, 0x28, 0x1d, 0xfe, 0x12, 0x8a, 0xe0, 0xc1, 0x02, 0xeb, 0xa4, 0x35,
     0x52, 0x88, 0xac, 0x44, 0x1a, 0x44, 0x82, 0x97, 0x4f, 0x5f, 0xa8, 0xd8, 0x9f, 0x67, 0x38, 0xa8,
     0x64, 0xb6, 0x62, 0x99, 0xbd, 0x96, 0x3c, 0xf5, 0x86, 0x09, 0x5c, 0x97, 0x6b, 0x8f, 0xae, 0xe0,
     0x60, 0xe7, 0x23, 0x98, 0x6a, 0xee, 0xc1, 0xb0, 0x14, 0xbe, 0x46, 0x2c, 0xfb, 0xa7, 0x27, 0x73,
     0xe4, 0xf3, 0x26, 0x33, 0xba, 0x99, 0xd4, 0x01, 0x38, 0xa8, 0xf2, 0x9e, 0x87, 0xe0, 0x71, 0x0b,
     0x25, 0x44, 0x07, 0x35, 0x88, 0xab, 0x67, 0x27, 0x56, 0x0e, 0xb5, 0xb5, 0xe8, 0x27, 0xb4, 0x49,
     0xdc, 0xb8, 0x48, 0x31, 0xff, 0x99, 0x48, 0xab, 0x11, 0xb4, 0xa0, 0xdf, 0x8a, 0x6d, 0xff, 0x43,
     0x69, 0x32, 0xa7, 0xbc, 0x63, 0x9d, 0x0f, 0xe0, 0x95, 0x34, 0x36, 0x25, 0x4b, 0x3e, 0x36, 0xbd,
     0x81, 0x91, 0x0b, 0x91, 0x9f, 0x3a, 0x04, 0xa2, 0x44, 0x28, 0x19, 0xa1, 0x38, 0x21, 0x4f, 0x25,
     0x59, 0x8a, 0x48, 0xc2};

const std::string DummyPib::SCHEME = "pib-dummy";
const std::string DummyTpm::SCHEME = "tpm-dummy";

NDN_CXX_V2_KEYCHAIN_REGISTER_PIB_BACKEND(DummyPib);
NDN_CXX_V2_KEYCHAIN_REGISTER_TPM_BACKEND(DummyTpm);

DummyPib::DummyPib(const std::string& locator)
{
}

void
DummyPib::setTpmLocator(const std::string& tpmLocator)
{
  m_tpmLocator = tpmLocator;
}

std::string
DummyPib::getTpmLocator() const
{
  return m_tpmLocator;
}

bool
DummyPib::hasIdentity(const Name& identityName) const
{
  return true;
}

void
DummyPib::addIdentity(const Name& identityName)
{
}

void
DummyPib::removeIdentity(const Name& identity)
{
}

void
DummyPib::clearIdentities()
{
}

std::set<Name>
DummyPib::getIdentities() const
{
  std::set<Name> identities;
  identities.insert("/dummy");
  return identities;
}

void
DummyPib::setDefaultIdentity(const Name& identityName)
{
}

Name
DummyPib::getDefaultIdentity() const
{
  return "/dummy";
}

bool
DummyPib::hasKey(const Name& keyName) const
{
  return true;
}

void
DummyPib::addKey(const Name& identity, const Name& keyName,
                 const uint8_t* key, size_t keyLen)
{
}

void
DummyPib::removeKey(const Name& keyName)
{
}

Buffer
DummyPib::getKeyBits(const Name& keyName) const
{
    typedef boost::iostreams::stream<boost::iostreams::array_source> arrayStream;
    arrayStream
    is(reinterpret_cast<const char*>(DUMMY_CERT), sizeof(DUMMY_CERT));
    auto cert = io::load<v2::Certificate>(is, io::BASE64);
    return cert->getPublicKey();
}

std::set<Name>
DummyPib::getKeysOfIdentity(const Name& identity) const
{
  std::set<Name> keys;
  keys.insert("/dummy/KEY/-%9C%28r%B8%AA%3B%60");
  return keys;
}

void
DummyPib::setDefaultKeyOfIdentity(const Name& identity, const Name& keyName)
{
}

Name
DummyPib::getDefaultKeyOfIdentity(const Name& identity) const
{
  return "/dummy/KEY/-%9C%28r%B8%AA%3B%60";
}

bool
DummyPib::hasCertificate(const Name& certName) const
{
  return true;
}

void
DummyPib::addCertificate(const v2::Certificate& certificate)
{
}

void
DummyPib::removeCertificate(const Name& certName)
{
}

v2::Certificate
DummyPib::getCertificate(const Name& certificateName) const
{
  static shared_ptr<v2::Certificate> cert = nullptr;
  if (cert == nullptr) {
    typedef boost::iostreams::stream<boost::iostreams::array_source> arrayStream;
    arrayStream
    is(reinterpret_cast<const char*>(DUMMY_CERT), sizeof(DUMMY_CERT));
    cert = io::load<v2::Certificate>(is, io::BASE64);
  }

  return *cert;
}

std::set<Name>
DummyPib::getCertificatesOfKey(const Name& keyName) const
{
  std::set<Name> certs;
  certs.insert("/dummy/KEY/-%9C%28r%B8%AA%3B%60/self/%FD%00%00%01%5E%DF%3Bv%01");
  return certs;
}

void
DummyPib::setDefaultCertificateOfKey(const Name& keyName, const Name& certName)
{
}

v2::Certificate
DummyPib::getDefaultCertificateOfKey(const Name& keyName) const
{
  static shared_ptr<v2::Certificate> cert = nullptr;
  if (cert == nullptr) {
    typedef boost::iostreams::stream<boost::iostreams::array_source> arrayStream;
    arrayStream
    is(reinterpret_cast<const char*>(DUMMY_CERT), sizeof(DUMMY_CERT));
    cert = io::load<v2::Certificate>(is, io::BASE64);
  }

  return *cert;
}

std::string
DummyPib::getScheme()
{
  return DummyPib::SCHEME;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

namespace tpm {

DummyKeyHandle::DummyKeyHandle(shared_ptr<transform::PrivateKey> key)
{
}

ConstBufferPtr
DummyKeyHandle::doSign(DigestAlgorithm digestAlgorithm, const uint8_t* buf, size_t size) const
{
  return make_shared<Buffer>(DUMMY_SIGNATURE, sizeof(DUMMY_SIGNATURE));
}

ConstBufferPtr
DummyKeyHandle::doDecrypt(const uint8_t* cipherText, size_t cipherTextLen) const
{
  throw Error("Not supported");
}

ConstBufferPtr
DummyKeyHandle::doDerivePublicKey() const
{
  throw Error("Not supported");
}

} // namespace tpm

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

DummyTpm::DummyTpm(const std::string& locator)
{
}

bool
DummyTpm::isTerminalMode() const
{
  return false;
}

void
DummyTpm::setTerminalMode(bool isTerminal) const
{
}

bool
DummyTpm::isTpmLocked() const
{
  return false;
}

bool
DummyTpm::doHasKey(const Name& keyName) const
{
  return false;
}

unique_ptr<tpm::KeyHandle>
DummyTpm::doGetKeyHandle(const Name& keyName) const
{
  unique_ptr<tpm::KeyHandle> m_dummyKeyHandle = make_unique<tpm::DummyKeyHandle>(nullptr);
  return m_dummyKeyHandle;
}

unique_ptr<tpm::KeyHandle>
DummyTpm::doCreateKey(const Name& identity, const KeyParams& params)
{
  unique_ptr<tpm::KeyHandle> m_dummyKeyHandle = make_unique<tpm::DummyKeyHandle>(nullptr);
  return m_dummyKeyHandle;
}

void
DummyTpm::doDeleteKey(const Name& keyName)
{
  throw Error("Not supported");
}

ConstBufferPtr
DummyTpm::doExportKey(const Name& keyName, const char* pw, size_t pwLen)
{
  throw Error("Not supported");
}

void
DummyTpm::doImportKey(const Name& keyName, const uint8_t* pkcs8, size_t pkcs8Len, const char* pw, size_t pwLen)
{
  throw Error("Not supported");
}

std::string
DummyTpm::getScheme()
{
  return DummyTpm::SCHEME;
}

} // namespace security
} // namespace ndn
