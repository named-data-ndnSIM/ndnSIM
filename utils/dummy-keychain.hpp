/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2014 University of California, Los Angeles
 *
 * See AUTHORS.md for complete list of authors and contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP
#define NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP

#include <ndn-cxx/security/key-chain.hpp>

namespace ndn {
namespace security {

class DummyPublicInfo : public SecPublicInfo {
public:
  virtual bool
  doesIdentityExist(const Name& identityName);

  virtual void
  addIdentity(const Name& identityName);

  virtual bool
  revokeIdentity();

  virtual bool
  doesPublicKeyExist(const Name& keyName);

  virtual void
  addKey(const Name& keyName, const PublicKey& publicKey);

  virtual shared_ptr<PublicKey>
  getPublicKey(const Name& keyName);

  virtual KeyType
  getPublicKeyType(const Name& keyName);

  virtual bool
  doesCertificateExist(const Name& certificateName);

  virtual void
  addCertificate(const IdentityCertificate& certificate);

  virtual shared_ptr<IdentityCertificate>
  getCertificate(const Name& certificateName);

  virtual Name
  getDefaultIdentity();

  virtual Name
  getDefaultKeyNameForIdentity(const Name& identityName);

  virtual Name
  getDefaultCertificateNameForKey(const Name& keyName);

  virtual void
  getAllIdentities(std::vector<Name>& nameList, bool isDefault);

  virtual void
  getAllKeyNames(std::vector<Name>& nameList, bool isDefault);

  virtual void
  getAllKeyNamesOfIdentity(const Name& identity, std::vector<Name>& nameList, bool isDefault);

  virtual void
  getAllCertificateNames(std::vector<Name>& nameList, bool isDefault);

  virtual void
  getAllCertificateNamesOfKey(const Name& keyName, std::vector<Name>& nameList, bool isDefault);

  virtual void
  deleteCertificateInfo(const Name& certificateName);

  virtual void
  deletePublicKeyInfo(const Name& keyName);

  virtual void
  deleteIdentityInfo(const Name& identity);

protected:
  virtual void
  setDefaultIdentityInternal(const Name& identityName);

  virtual void
  setDefaultKeyNameForIdentityInternal(const Name& keyName);

  virtual void
  setDefaultCertificateNameForKeyInternal(const Name& certificateName);
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class DummyTpm : public SecTpm {
public:
  virtual void
  setTpmPassword(const uint8_t* password, size_t passwordLength);

  virtual void
  resetTpmPassword();

  virtual void
  setInTerminal(bool inTerminal);

  virtual bool
  getInTerminal() const;

  virtual bool
  isLocked();

  virtual bool
  unlockTpm(const char* password, size_t passwordLength, bool usePassword);

  virtual void
  generateKeyPairInTpm(const Name& keyName, const KeyParams& params);

  virtual void
  deleteKeyPairInTpm(const Name& keyName);

  virtual shared_ptr<PublicKey>
  getPublicKeyFromTpm(const Name& keyName);

  virtual Block
  signInTpm(const uint8_t* data, size_t dataLength, const Name& keyName,
            DigestAlgorithm digestAlgorithm);

  virtual ConstBufferPtr
  decryptInTpm(const uint8_t* data, size_t dataLength, const Name& keyName, bool isSymmetric);

  virtual ConstBufferPtr
  encryptInTpm(const uint8_t* data, size_t dataLength, const Name& keyName, bool isSymmetric);

  virtual void
  generateSymmetricKeyInTpm(const Name& keyName, const KeyParams& params);

  virtual bool
  doesKeyExistInTpm(const Name& keyName, KeyClass keyClass);

  virtual bool
  generateRandomBlock(uint8_t* res, size_t size);

  virtual void
  addAppToAcl(const Name& keyName, KeyClass keyClass, const std::string& appPath, AclType acl);

protected:
  virtual ConstBufferPtr
  exportPrivateKeyPkcs8FromTpm(const Name& keyName);

  virtual bool
  importPrivateKeyPkcs8IntoTpm(const Name& keyName, const uint8_t* buffer, size_t bufferSize);

  virtual bool
  importPublicKeyPkcs1IntoTpm(const Name& keyName, const uint8_t* buffer, size_t bufferSize);
};

typedef KeyChainTraits<DummyPublicInfo, DummyTpm> DummyKeyChainTraits;

} // namespace security

class DummyKeyChain : public KeyChain {
public:
  DummyKeyChain()
    : KeyChain(security::DummyKeyChainTraits())
  {
  }
};

} // namespace ndn

#endif // NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP
