.. _ndnSIM packet format:

ndnSIM packet format
====================

Basic field types
+++++++++++++++++

::

	uint8_t ::= 8-bit unsigned integer

	uint16_t ::= 16-bit unsigned integer

	uint32_t ::= 32-bit unsigned integer

	CHAR ::= 8-bit binary data

	Blob ::= Length CHAR{Length}

	Length ::= uint16_t

NDN Packet Type
+++++++++++++++

::

	Packet ::= Version 
		   PacketType
                   Length
		   (Interest | ContentObject)

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |    Version    |   PacketType  |            Length             |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

For ccnb-encoding compatibility, ``Version`` / ``PacketType`` has two reserved values to denote ccnb-encoded packet:

Version 0x01, PacketType 0xD2 --- ccnb-encoded ``Interest`` packet
Version 0x04, PacketType 0x82 --- ccnb-encoded ``ContentObject`` packet


Version
~~~~~~~

::

	Version ::= uint8_t 

The current version of the packet format.  Right only value 0x80 is allowed.

PacketType
~~~~~~~~~~

::

	PacketType ::= uint8_t  

In the current version, two packet types are defined:

- ``Interest`` (``PacketType`` = 0)
- ``ContentObject`` (``PacketType`` = 1)

Any other value of PacketType is invalid and such a packet should be discarded.

Interest
++++++++

The objective of the new format is to optimize encoding/decoding operations.

::

	Interest ::= Nonce 
	     	     Scope
                     NackType
		     InterestLifetime 
	     	     Name 
	     	     Selectors 
	     	     Options

Minumum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 0) + (2 + 0) + (2 + 0) = 14

Maximum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 65535) + (2 + 65535) + (2 + 65535) = 196619

::

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                          Nonce                                |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |     Scope     |   NackType    |      InterestLifetime         |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                            Name                               ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                          Selectors                            ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                          Options                              ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


Nonce
~~~~~

::

	Nonce ::= uint32_t

NackType
~~~~~~~~

::

	NackType := uint8_t

Currently, ndnSIM defines following NackTypes:

- 0: NORMAL_INTEREST
- 10: NACK_LOOP
- 11: NACK_CONGESTION
- 12: NACK_GIVEUP_PIT

Values 128-255 are reserved for any application-specific and experimental purposes.


InterestLifetime
~~~~~~~~~~~~~~~~

::

	InterestLifetime := uint16_t 

Interest lifetime is specified in seconds.  Max value is about 18 hours.

Scope
~~~~~

::

	Scope ::= uint8_t 

Scope 0 prevents propagation beyond the local ccnd (even to other applications on the same host). Scope 1 limits propagation to the applications on the originating host. Scope 2 limits propagation to no further than the next host.

Name
~~~~

::

	Name ::= Length (NameComponent)*

	NameComponent ::= Blob


Selectors
~~~~~~~~~

::

	Selectors ::= Length (SelectorType Selector)*

        SelectorType ::= uint8_t

	Selector ::= MinSuffixComponents | MaxSuffixComponents | Publisher | Exclude | ChildSelector | AnswerOriginKind

Currently, ndnSIM defines following SelectorTypes:

- 0x01: Exclude

Other types are currently undefined

Exclude
~~~~~~~

::

	Exclude ::= Length (ExcludeComponent)*

        ExcludeComponent ::= ExcludeNameType NameComponent ExcludeAnyType? |
                             ExcludeAnyType

        ExcludeNameType ::= uint8_t  (==0x01)

        ExcludeAnyType ::= uint8_t   (==0x02)

Options
~~~~~~~

::

	Options ::= Length (Option)*


.. .................................................................................................. ..
.. .................................................................................................. ..
.. .................................................................................................. ..
.. .................................................................................................. ..


ContentObject
+++++++++++++

::

	ContentObject ::= Name
                   	  Content
                          Signature

::

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                             Name                              ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                           Content                             ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
        ~                                                               ~
        ~                           Signature                           ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


Signature
~~~~~~~~~

::

	Signature ::= Length 
		      SignatureType
		      <type-dependeds signature data>

Length specifies cumulative size of SignatureInfo and SignatureBits.  If SignatureType is not recognized, the received can either discard the packet or ignore the signature using Length field, specified combined length of SignatureType and SignatureType-dependent fields.

::

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |           Length              |         SignatureType         |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        ~                                                               ~
        ~               <type-dependeds signature data>                 ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


SignatureType
^^^^^^^^^^^^^

::

	SignatureType ::= uint16_t

The current version specifies three type of signatures:

- 0x0000: empty signature
- 0x0001: SHA256 (not a real signature, but just a digest of the content)
- 0x0002: SHA256withRSA (real public-key signature based on SHA-256 digest)

Other values may be defined in future.

- Values greater or equal to 0xFF00 are for experimental purposes (e.g., for simulations)

<type-dependeds signature data>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

+-------------------+---------------------------+
| SignatureType     |  SignatureData definition |
+===================+===========================+
| 0 (empty)         |  empty sequence           |
+-------------------+---------------------------+
| 1 (sha256)        |  CHAR{32}                 |
+-------------------+---------------------------+
| 2 (SHA256withRSA) |  CHAR{32} KeyLocator      |
+-------------------+---------------------------+

KeyLocator
^^^^^^^^^^

::

	KeyLocator ::= CertName
		       
	CertName ::= Name


Content
^^^^^^^

::

	Content ::= Length
		    ContentInfo
		    ContentData

Content length can be computed as:  Length - (1 - ContentInfo.Length)

::

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |            Length             |       Length (content Info)   |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                           Timestamp                           |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |           Freshness           |           Reserved            |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |  Length (ContentInfoOptions)  |                               |
	|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
        ~                                                               ~
        ~                       ContentInfoOptions                      ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        ~                                                               ~
        ~                         ContentData                           ~
        |							        |	
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


ContentInfo
^^^^^^^^^^^

::

	ContentInfo ::= Length 
			Timestamp
			Freshness
			ContentOptions

Timestamp
~~~~~~~~~

::

	Timestamp ::= uint32_t

Timestamp specifies content generation time as Unix time timestamp (number of seconds since midnight 1/1/1970).

Freshness
~~~~~~~~~

::

	Freshness ::= uint16_t

Freshness specifies time in seconds (since Timestamp) for which the content is considered valid.  

Value 0xFFFF means that content is always valid.


ContentOptions
~~~~~~~~~~~~~~

::

	ContentOptions ::= Length
			   ContentOption*

	ContentOption ::= Type |
			  FinalBlockID


Not currently defined

