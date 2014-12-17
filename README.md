ndnSIM 2.0
==========

ndnSIM 2.0 is a new release of [NS-3 based Named Data Networking (NDN)
simulator](http://ndnsim.net/1.0/) that went through extensive refactoring and rewriting.
The key new features of the new version:

- ndnSIM no longer re-implements basic NDN primitives and directly uses implementation from
  [ndn-cxx library (NDN C++ library with eXperimental eXtensions)](http://named-data.net/doc/ndn-cxx/)

- All NDN forwarding and management is implemented directly using source code of
  [Named Data Networking Forwarding Daemon (NFD)](http://named-data.net/doc/NFD/)

- Packet format changed to [NDN-TLV](http://named-data.net/doc/ndn-tlv/)

[ndnSIM 2.0 documentation](http://ndnsim.net)
---------------------------------------------

For more information, including downloading and compilation instruction, please refer to
http://ndnsim.net or documentation in `docs/` folder.
