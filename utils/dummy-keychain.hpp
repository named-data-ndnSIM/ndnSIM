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

#ifndef NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP
#define NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP

#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/security-common.hpp>
#include <ndn-cxx/security/pib/pib-impl.hpp>
#include <ndn-cxx/security/tpm/back-end.hpp>
#include <ndn-cxx/security/transform/private-key.hpp>

namespace ndn {
namespace security {

using pib::PibImpl;
using tpm::BackEnd;
using tpm::KeyHandle;

class DummyPib : public PibImpl
{
public:
  class Error : public PibImpl::Error
  {
  public:
    explicit
    Error(const std::string& what)
      : PibImpl::Error(what)
    {
    }
  };

public:
  explicit DummyPib(const std::string& locator);

  // TPM management
  void
  setTpmLocator(const std::string& tpmLocator) override;

  std::string
  getTpmLocator() const override;

  // Identity manangement
  bool
  hasIdentity(const Name& identityName) const override;

  void
  addIdentity(const Name& identityName) override;

  void
  removeIdentity(const Name& identity) override;

  void
  clearIdentities() override;

  std::set<Name>
  getIdentities() const override;

  void
  setDefaultIdentity(const Name& identityName) override;

  Name
  getDefaultIdentity() const override;

  // Key management
  bool
  hasKey(const Name& keyName) const override;

  void
  addKey(const Name& identity, const Name& keyName, const uint8_t* key,
         size_t keyLen) override;

  void
  removeKey(const Name& keyName) override;

  Buffer
  getKeyBits(const Name& keyName) const override;

  std::set<Name>
  getKeysOfIdentity(const Name& identity) const override;

  void
  setDefaultKeyOfIdentity(const Name& identity, const Name& keyName) override;

  Name
  getDefaultKeyOfIdentity(const Name& identity) const override;

  // certificate management
  bool
  hasCertificate(const Name& certName) const override;

  void
  addCertificate(const v2::Certificate& certificate) override;

  void
  removeCertificate(const Name& certName) override;

  v2::Certificate
  getCertificate(const Name& certificateName) const override;

  std::set<Name>
  getCertificatesOfKey(const Name& keyName) const override;

  void
  setDefaultCertificateOfKey(const Name& keyName, const Name& certName) override;

  v2::Certificate
  getDefaultCertificateOfKey(const Name& keyName) const override;

  static std::string
  getScheme();

  static const std::string SCHEME;

private:
  std::string m_tpmLocator;
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

namespace tpm {

class DummyKeyHandle : public KeyHandle
{
public:
  explicit
  DummyKeyHandle(shared_ptr<transform::PrivateKey> key);

private:
  ConstBufferPtr
  doSign(DigestAlgorithm digestAlgorithm, const uint8_t* buf, size_t size) const final;

  ConstBufferPtr
  doDecrypt(const uint8_t* cipherText, size_t cipherTextLen) const final;

  ConstBufferPtr
  doDerivePublicKey() const final;
};

} // namespace tpm

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class DummyTpm : public BackEnd
{
public:
  class Error : public BackEnd::Error
  {
  public:
    explicit
    Error(const std::string& what)
      : BackEnd::Error(what)
    {
    }
  };

public:
  explicit DummyTpm(const std::string& locator);

  bool
  isTerminalMode() const override;

  void
  setTerminalMode(bool isTerminal) const override;

  bool
  isTpmLocked() const override;

  ConstBufferPtr
  sign(const uint8_t* buf, size_t size, const Name& keyName, DigestAlgorithm digestAlgorithm) const;

  static std::string
  getScheme();

private:
  bool
  doHasKey(const Name& keyName) const final;

  unique_ptr<tpm::KeyHandle>
  doGetKeyHandle(const Name& keyName) const final;

  unique_ptr<tpm::KeyHandle>
  doCreateKey(const Name& identity, const KeyParams& params) final;

  void
  doDeleteKey(const Name& keyName) final;

  ConstBufferPtr
  doExportKey(const Name& keyName, const char* pw, size_t pwLen) final;

  void
  doImportKey(const Name& keyName, const uint8_t* pkcs8, size_t pkcs8Len, const char* pw, size_t pwLen) final;

public:
  static const std::string SCHEME;
};

} // namespace security
} // namespace ndn

#endif // NDNSIM_UTILS_DUMMY_KEYCHAIN_HPP
