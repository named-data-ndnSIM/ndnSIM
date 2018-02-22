ndnSIM research papers
======================

Here is a list of ndnSIM-related papers.  If you want your paper to appear in this list, please send an email to our mailing list or to us directly.

General papers about ndnSIM
---------------------------

.. note::
    if you refer to ndnSIM in a published work, please cite the following papers, not just the ndnSIM website.

**For ndnSIM >= 2.0:**

- **S. Mastorakis, A. Afanasyev, and L. Zhang, "On the Evolution of ndnSIM: an Open-Source Simulator for NDN Experimentation," ACM SIGCOMM Computer Communication Review (CCR), July 2017**  (`PDF <https://ccronline.sigcomm.org/wp-content/uploads/2017/08/sigcomm-ccr-final112-full-letter.pdf>`__, `BibTex <http://web.cs.ucla.edu/~mastorakis/bibtex-ndnsim>`__)

    As a proposed Internet architecture, Named Data Networking (NDN) takes a
    fundamental departure from today's TCP/IP architecture, thus requiring extensive
    experimentation and evaluation. To facilitate such experimentation, we have developed
    ndnSIM, an open-source NDN simulator based on the NS-3 simulation framework. Since its
    first release in 2012, ndnSIM has gone through five years of active development and
    integration with the NDN prototype implementations, and has become a popular platform
    used by hundreds of researchers around the world. This paper presents an overview of the
    ndnSIM design, the ndnSIM development process, the design tradeoffs, and the reasons
    behind the design decisions. We also share with the community a number of lessons we
    have learned in the process.

- **S. Mastorakis, A. Afanasyev, I. Moiseenko, and L. Zhang, "ndnSIM 2.0: A new version of the NDN simulator for NS-3," NDN, Technical Report NDN-0028, 2015** (`PDF <http://named-data.net/techreport/ndn-0028-1-ndnsim-v2.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/399>`__)

    The fundamental departure of the Named-Data Networking (NDN) communication paradigm from the IP
    principles requires extensive evaluation through experimentation, and simulation is a necessary
    tool to enable the experimentation at scale.  We released the first version of ndnSIM, an open
    source NS-3-based NDN simulator, back in June 2012.  Since then, ndnSIM has undergone
    substantial development resulting in ndnSIM 2.0, which was released in January 2015.  This paper
    reports the design and features of this new simulator version.  The goal of the new release is
    to match the simulation platform to the latest advancements of NDN research.  Therefore, it uses
    the ndn-cxx library (NDN C++ library with eXperimental eXtensions) and the NDN Forwarding Daemon
    (NFD) to enable experiments with real code in a simulation environment.

**For ndnSIM 1.0:**

- **A. Afanasyev, I. Moiseenko, and L. Zhang, "ndnSIM: NDN simulator for NS-3," NDN, Technical Report NDN-0005, 2012** (`PDF <http://named-data.net/wp-content/uploads/TRndnsim.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/367>`__)

    Named Data Networking (NDN) is a newly proposed Internet architecture.  NDN retains the
    Internet's hourglass architecture but evolves the thin waist.  Instead of pushing data to
    specific locations, NDN retrieves data by name.  On one hand, this simple change allows NDN
    networks to use almost all of the Internet's well tested engineering properties to solve not
    only IP's communication problems but also digital distribution and control problems.  On the
    other hand, a distribution architecture differs in fundamental ways from a point-to-point
    communication architecture of today's Internet and raises many new research challenges.
    Simulation can serve as a flexible tool to examine and evaluate various aspects of this new
    architecture.  To provide the research community at large with a common simulation platform, we
    have developed an open source NS-3 based simulator, ndnSIM, which faithfully implemented the
    basic components of a NDN network in a modular way.  This paper provides an overview of ndnSIM.


Research papers that use ndnSIM
-------------------------------

#. **L. Wang, A. Afanasyev, R. Kuntz, R. Vuyyuru, R. Wakikawa, and L. Zhang, "Rapid Traffic Information Dissemination Using Named Data," in Proceedings of the 1st ACM workshop on Emerging Name-Oriented Mobile Networking Design - Architecture, Algorithms, and Applications (NoM'12), Hilton Head Island, South Carolina, June 2012, pp. 7–12.** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Wang/nom.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/365>`__, `simulation code <https://github.com/cawka/ndnSIM-nom-rapid-car2car>`__)

    Our previous work applied the Named Data Networking approach to vehicle-to-vehicle (V2V) communications and developed a simple design for traffic information dissemination applications. This paper uses simulations to evaluate the feasibility of the design as described in [1].
    Our results show that data names can greatly facilitate the forwarding process for Interest and data packets.
    With adequate vehicle density, data can propagate over long distances robustly at tens of kilometers per second and a requester can retrieve the desired traffic information 10km away in a matter of seconds.

#. **Z. Zhu, C. Bian, A. Afanasyev, V. Jacobson, and L. Zhang, "Chronos: Serverless Multi-User Chat Over NDN," NDN, Technical Report NDN-0008, 2012.** (`PDF <http://named-data.net/techreport/TR008-chronos.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/371>`__)

    Multi-user applications are commonly implemented using a centralized server.
    This paper presents a new design for multi-user chat applications (Chronos) that works in a distributed, serverless fashion over Named Data Networking.
    In Chronos, all participants share their views by exchanging the cryptographic digests of the chat room data set.
    A newly generated message causes a change of the digest at the message originator, which leads to retrieving the new data by all other participants in an efficient way and resynchronization of chat room views.
    Chronos does not have a single point of failure and eliminates traffic concentration problem of server-based implementations.
    We use simulations to evaluate and compare Chronos with a traditional server-based chat room implementation.
    Our results demonstrate Chronos' robustness and efficiency in data dissemination.
    Chronos' approach of replacing centralized servers by distributed data synchronization can be applied to a variety of distributed applications to simplify design and ease deployment.

#. **M. Vahlenkamp, "Threats on Information-Centric Networking", Hamburg University of Applied Sciences, Technical Report, 2012.** (`PDF <http://inet.cpt.haw-hamburg.de/teaching/ws-2012-13/master-projekt/markus-vahlenkamp_seminar.pdf>`__)

    The ICN approach aims for reflecting these changes in usage of the Internet and is thus dragging content awareness into the network, for instance to let the network itself decide where to acquire requested data from and thereby utilize content caches to increase the data dissemination efficiency. All this is backed by the use of the publish/subscribe paradigm that is utilised to announce content availability and request its delivery. Since the ICN paradigm is entirely different from todays Internet, new challenges arise within the area of network security. NDN/CCNx, as the most popular ICN approach, claims to solve a couple of different security flaws which the actual Internet is suffering from. This raises the questions of which vulnerabilities still exist and if maybe new issues arise.

#. **C. Yi, A. Afanasyev, I. Moiseenko, L. Wang, B. Zhang, and L. Zhang, "A Case for Stateful Forwarding Plane," Computer Communications, vol. 36, no. 7, pp. 779–791, 2013. ISSN 0140-3664** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Yi/comcom-stateful-forwarding.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/380>`__, `simulation code <https://github.com/cawka/ndnSIM-comcom-stateful-fw>`__)

    In Named Data Networking (NDN), packets carry data names instead of source and destination addresses.
    This paradigm shift leads to a new network forwarding plane: data consumers send *Interest* packets to request desired data, routers forward Interest packets and maintain the state of all pending Interests, which is then used to guide *Data* packets back to the consumers.
    Maintaining the pending Interest state, together with the two-way Interest and Data exchange, enables NDN routers' *forwarding* process to measure performance of different paths, quickly detect failures and retry alternative paths.
    In this paper we describe an initial design of NDN's forwarding plane and evaluate its data delivery performance under adverse conditions.
    Our results show that this stateful forwarding plane can successfully circumvent prefix hijackers, avoid failed links, and utilize multiple paths to mitigate congestion.
    We also compare NDN's performance with that of IP-based solutions to highlight the advantages of a stateful forwarding plane.

#. **A. Afanasyev, P. Mahadevan, I. Moiseenko, E. Uzun, and L. Zhang, "Interest Flooding Attack and Countermeasures in Named Data Networking," in Proc. of IFIP Networking 2013, May 2013.** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ifip-interest-flooding-ndn.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/381>`__, `simulation code <https://github.com/cawka/ndnSIM-ddos-interest-flooding>`__)

    Distributed Denial of Service (DDoS) attacks are an ongoing problem in today's Internet, where packets from a large number of compromised hosts thwart the paths to the victim site and/or overload the victim machines.
    In a newly proposed future Internet architecture, Named Data Networking (NDN), end users request desired data by sending Interest packets, and the network delivers Data packets upon request only, effectively eliminating many existing DDoS attacks.
    However, an NDN network can be subject to a new type of DDoS attack, namely Interest packet flooding.
    In this paper we investigate effective solutions to mitigate Interest flooding.
    We show that NDN's inherent properties of storing per packet state on each router and maintaining flow balance (i.e., one Interest packet retrieves at most one Data packet) provides the  basis for effective DDoS mitigation algorithms.
    Our evaluation through simulations shows that the solution can quickly and effectively respond and mitigate Interest flooding.

#. **B. Zhou, C. Wu, X. Hong, and M. Jiang, "Algorithms for Distributed Programmable Controllers", Technical Report, March 2013.** (`PDF <http://hong.cs.ua.edu/DCP-techReport-March2013.pdf>`__)

    A few works on SDN (Software-Defined Networking) like those in Onix improve programmability of the distributed network control.
    The asynchronism and Byzantine issues of the control challenge the re-configurability of the service that is to safely program the control in atomic so as to avoid the transient control issues like the routing loops and black holes.
    We propose two important algorithms of the distributed control to enable the programmability: (1) the reconfiguration primitive allows the network control of the services being able to safely react to an external event; and (2) the reuse primitive allows the control states of a service being accessible for all services. We give concepts and algorithms of two primitives.
    In addition, we provide the concrete cases of the current approaches for ICN (Information-Centric Networking) and CDN (Content Distribution Networks) for quests of the reconfigurability and programmability.
    Then, we evaluate the performance of ICN in both simulation and the PlanetLab testbed.
    The evaluation results show that the layer improves the lowers 19.6% of the Interest delays in the ICN that is heavily congested and lowers 97% delays in the PlanetLab with 9 nodes on usual case.
    In addition, the evaluation of CDN on the PlanetLab shows that it reduces 81% request delay on usual case.

#. **M. Tortelli, L. A. Grieco, and G. Boggia, "Performance Assessment of Routing Strategies in Named Data Networking", in Proc. of GTTI 2013 Session on Telecommunication Networks, 2013** (`PDF <http://www.gtti.it/GTTI13/papers/Tortelli_et_al_GTTI2013.pdf>`__)

    Information Centric Networking (ICN) architectures are currently being investigated to orient the Future Internet towards a content centric paradigm, thus allowing the provisioning of more secure, efficient, and scalable services.
    In this work, we focus on the Named Data Networking (NDN) proposal to analyze the impact of several routing and forwarding strategies, which play a fundamental role in ICN.
    In particular, thanks to the recently devised ns-3 based NDN simulator, namely ndnSIM, we conduce an extensive simulation campaign using the GEANT topology as a core network.
    We monitor different distinctive metrics, such as file download time, server load reduction, hit ratio, hit distance, and communication overhead, in different topologies and traffic conditions.
    Simulation results show that the election of a single best forwarding strategy is a difficult task.
    Indeed, the pros and cons of each strategy are heavily influenced by the popularity distribution of contents, which, in turn, affects the effectiveness of the distributed caching mechanisms typically used in the NDN architecture.

#. **S. Seo, J.-M. Kang, A. Leon-Garcia, Y. Han, and J. W.-K. Hong, "Secure and Efficient Context Data Collection using Content-Centric Networking", in Proc. of International Workshop on Smart Communication Protocols and Algorithms (SCPA), 2013** (`PDF <http://dpnm.postech.ac.kr/papers/SCPA/13/sesise/scpa13.pdf>`__)

    Context data collection is a fundamental and important process for realizing context-aware recommender or personalization systems.
    The existing context data collection approaches are based-on traditional TCP/IP that has several disadvantages such as lack of mobility and security.
    On the other hand, Content-Centric Networking (CCN) provides advantages in terms of mobility, security, and bandwidth efficiency compared to TCP/IP.
    In this paper, we propose a secure and efficient context data collection and provision approach based on CCN.
    Simulation results show that this approach can reduce bandwidth consumption by 52.7%–98.9% in comparison to a TCP/IP-based one.

#. **J. Ran, N. Lv, D. Zhang, Y. Ma, and Z. Xie, "On Performance of Cache Policies in Named Data Networking", in International Conference on Advanced Computer Science and Electronics Information (ICACSEI 2013), 2013** (`PDF <http://www.atlantis-press.com/php/download_paper.php?id=7640>`__)

    Named Data Network (NDN) is gaining increasingly concerns, as an important direction of the future Internet architecture research centered on content.
    Content caching has played a key role in NDN.
    Existing cache replacement policies like Least Frequently Used (LFU) and Least Recently Used (LRU) have failed to make full use of the popularity of contents, which leads to a low cache efficiency in the dynamic network.
    In order to make the best use of content popularity in the cache strategy, this paper has proposed a cache replacement policy based on content popularity (CCP), and designed the data structure format and replacement algorithm.
    For fully studying and analyzing the performance of different cache policies in NDN in terms of network throughput, server load and cache hit ratio, we have done a lot of simulations to show how they will improve the network.
    The simulation results show that our proposed CCP can significantly decrease the server load with a higher cache hit ratio and increase the network capacity at the same time compared with LRU and LFU.
    And the average throughput is reduced significantly by nearly 47% in comparison to that of the case without in-networking caching.
    Moreover, it also shows the performance under different sizes of content store.
    The effectiveness of the CCP strategy is proved during the simulation.

#. **M. Wahlisch, T.C. Schmidt, and M. Vahlenkamp, "Backscatter from the Data Plane--Threats to Stability and Security in Information-Centric Network Infrastructure", in Computer Networks, 2013** (`DOI 10.1016/j.comnet.2013.07.009 <http://dx.doi.org/10.1016/j.comnet.2013.07.009>`__)

    Information-centric networking (ICN) raises data objects to first class routable entities in the network and changes the Internet paradigm from host-centric connectivity to data-oriented delivery.
    However, current approaches to content routing heavily rely on data-driven protocol events and thereby introduce a strong coupling of the control to the data plane in the underlying routing infrastructure.
    In this paper, threats to the stability and security of the content distribution system are analyzed in theory, simulations, and practical experiments.
    We derive relations between state resources and the performance of routers, and demonstrate how this coupling can be misused in practice.
    We further show how state-based forwarding tends to degrade by decorrelating resources.
    We identify intrinsic attack vectors present in current content-centric routing, as well as possibilities and limitations to mitigate them.
    Our overall findings suggest that major architectural refinements are required prior to global ICN deployment in the real world.

#. **Xiaoke Jiang and Jun Bi, "Technical Report: Named Content Delivery Network", 2013** (`PDF <http://netarchlab.tsinghua.edu.cn/~shock/THU-NetArchLab-ICN-TR-nCDN-20130730.pdf>`__)

    CDN (Content Delivery Network) focuses on delivering requested data to users, no matter where the data comes from; but the fundamental goal of IP is to connect hosts.
    The essential mismatching leads to complexity and inefficiency.
    More specifically,
    1) CDN has to build components to map what to where, which is resource consuming;
    2) CDN has to monitor real-time network state on the application layer, which is complex and not accurate.
    In contrast, NDN (Named Data Networking), provides the information and function that traditional CDN devotes a great deal of effort to achieve, since NDN routes by name, its routing plane holds the "what", information of content distribution, and its stateful forwarding plane can detect and adapt to dynamic of the Internet.
    Thus this work enhances current CDN with NDN, here dubbed Named Content Delivery Network, or nCDN.
    In nCDN, CDN itself focuses on services such as accounting, data analysis etc; NDN runs over IP and takes charge of content routing and delivery.
    nCDN is more adaptive to the dynamic of the Internet and improves the performance, especially in a scenario where content copies are hosted in several hosts.
    nCDN makes it easier to implement optimization solutions and CDN Interconnecting. Our simulations demonstrate that nCDN is better than traditional CDN on almost all aspects, including the scalability, reliability, and QoS.

#. **Xiaoke Jiang, Jun Bi, Youchao Wang, and You Wang, "Interest Set Mechanism to Improve the Transport of Named Data Networking", in proceedings of ACM SIGCOMM13 (poster), Hongkong, China, 2013** (`PDF <http://netarchlab.tsinghua.edu.cn/~junbi/SIGCOMM2013-1.pdf>`__)

    Named Data Networking (NDN) is currently a hot research topic promising to be one of the most advanced developments in future Internet architectures.
    Researches have built real software systems over NDN which work on speciﬁed scenarios.
    In this paper, we proposal an new mechanism which aggregate similar Interest packets to improve the efficient of transport of NDN.
    Firstly we prove the optimal chunk size which simultaneously minimize the latency and maximize the valid payload ratio during a complete data acquiring process.
    That's why we aggregate the Interest packet but not increase the size of Data packet.
    Secondly we introduce the Interest Set mechanism that which is able to maintain a conversational "channel" between the data consumer and provider to cover the space and time uncertainty of data packet generating and at the same time reduces the number of FIB lookups and compresses the PIT.

#. **Z. Zhu and A. Afanasyev, "Let's ChronoSync: Decentralized Dataset State Synchronization in Named Data Networking," in Proceedings of the 21st IEEE International Conference on Network Protocols (ICNP 2013), Goettingen, Germany, October 2013** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Zhu/chronosync-icnp2013.pdf>`__)

    In supporting many distributed applications, such as group text messaging, file sharing, and joint editing, a basic requirement is the efficient and robust synchronization of knowledge about the dataset such as text messages, changes to the shared folder, or document edits.
    We propose ChronoSync protocol, which exploits the features of the Named Data Networking architecture to efficiently synchronize the state of a dataset among a distributed group of users.
    Using appropriate naming rules, ChronoSync summarizes the state of a dataset in a condensed cryptographic digest form and exchange it among the distributed parties.
    Differences of the dataset can be inferred from the digests and disseminated efficiently to all parties.
    With the complete and up-to-date knowledge of the dataset changes, applications can decide whether or when to fetch which pieces of the data.
    We implemented ChronoSync as a C++ library and developed two distributed application prototypes based on it.
    We show through simulations that ChronoSync is effective and efficient in synchronization dataset state, and is robust against packet losses and network partitions.

#. **Y. Wang, N. Rozhnova, A. Narayanan, D. Oran, and I. Rhee, "An improved hop-by-hop interest shaper for congestion control in named data networking". In Proceedings of the 3rd ACM SIGCOMM workshop on Information-centric networking, 2013** (`PDF <http://conferences.sigcomm.org/sigcomm/2013/papers/icn/p55.pdf>`__)

    Hop-by-hop interest shaping has been proposed as a viable congestion control mechanism in Named Data Networking (NDN).
    Interest shaping exploits the strict receiver-driven traffc pattern and the symmetric bidirectional forwarding in NDN to control the returning data rate.
    In this paper, we point out that both interests and contents contribute to congestion and their interdependence must be considered in any interest shaping algorithm.
    We first analyze this issue mathematically by formulating it as an optimization problem to obtain the optimal shaping rate.
    Then a practical interest shaping algorithm is proposed to achieve high link utilization without congestive data loss.
    We further note that ﬂow differentiation in NDN is complicated and design our scheme independently of traffc ﬂows.
    We demonstrate our hopby-hop interest shaper in conjunction with simple AdditiveIncrease-Multiplicative-Decrease (AIMD) clients using the ns3-based NDN simulator (ndnSIM).
    Our results show that the proposed shaping algorithm can effectively control congestion and achieve near-optimal throughput.

#. **S. Arianfar, P. Sarolahti, and J. Ott, "Deadline-based Resource Management for Information-Centric Networks". In Proceedings of the 3rd ACM SIGCOMM workshop on Information-centric networking, 2013** (`doi 10.1145/2491224.2491226 <http://dx.doi.org/10.1145/2491224.2491226>`__)

    Unlike in traditional IP-based end-to-end network sessions, in information-centric networks the data source may change during a communication session.
    Therefore the response time to subsequent data requests may vary signiﬁcantly depending on whether data comes from nearby cache, or a distant source.
    This is a complication for designing resource management, reliability and other algorithms, that traditionally use RTT measurements for determining when data is considered lost and should be retransmitted (along with related congestion control adjustments).
    This paper discusses a different approach for designing resource management in information-centric networks: data packets are assigned with a lifetime, that is used as a basis for scheduling and resource management in the network, and for congestion control and retransmission logic at the end hosts.
    We demonstrate an initial evaluation of this approach based on ns-3 simulations on CCN framework

#. **M. Conti, P. Gasti, M. Teoli, "A lightweight mechanism for detection of cache pollution attacks in Named Data Networking". Computer Networks, 2013** (`doi 10.1016/j.comnet.2013.07.034 <http://dx.doi.org/10.1016/j.comnet.2013.07.034>`__)

    Content-Centric Networking (CCN) is an emerging paradigm being considered as a possible replacement for the current IP-based host-centric Internet infrastructure.
    In CCN, named content---rather than addressable hosts---becomes a first-class entity.
    Content is therefore decoupled from its location.
    This allows, among other things, the implementation of ubiquitous caching.

    Named-Data Networking (NDN) is a prominent example of CCN.
    In NDN, all nodes (i.e., hosts, routers) are allowed to have a local cache, used to satisfy incoming requests for content.
    This makes NDN a good architecture for efficient large scale content distribution.
    However, reliance on caching allows an adversary to perform attacks that are very effective and relatively easy to implement.
    Such attacks include cache poisoning (i.e., introducing malicious content into caches) and cache pollution (i.e., disrupting cache locality).

    This paper focuses on cache pollution attacks, where the adversary’s goal is to disrupt cache locality to increase link utilization and cache misses for honest consumers.
    We show, via simulations, that such attacks can be implemented in NDN using limited resources, and that their effectiveness is not limited to small topologies.
    We then illustrate that existing proactive countermeasures are ineffective against realistic adversaries. Finally, we introduce a new technique for detecting pollution attacks.
    Our technique detects high and low rate attacks on different topologies with high accuracy.

#. **G. Mauri and G. Verticale, "Distributing key revocation status in Named Data Networking". Advances in Communication Networking, pages 310–313. Springer, 2013** (`doi 10.1007/978-3-642-40552-5_31 <http://dx.doi.org/10.1007/978-3-642-40552-5_31>`__)

    Content Centric Networking (CCN) is a new network paradigm designed to satisfy user needs considering the growth of data demand.
    Named Data Networking (NDN) is a research project that is developing the future Internet architecture using the principles behind CCN.
    In this novel architecture, the contents are addressed by their name and not by their location.
    Thus, the attention is shifted from user to content, resulting in a caching network that is more efficient and flexible than an IP network for content distribution and management with beneficial effects on timely delivery.
    In NDN, the content objects are divided into chunks, each digitally signed by its producer, and most papers assume that verification is made only by the content consumer.
    In order to perform signature verification, a node needs the signer’s key, which can be easily retrieved by issuing a standard interest message.
    Although content verification at the end node prevents disruptive attacks in which false data is delivered to applications, the verification of key validity is also necessary.
    Otherwise, false data would be cached and forwarded instead of correct data resulting in a denial of service and paving the way for more sophisticated attacks.

#. **W. Drira and F. Filali, "A Pub/Sub extension to NDN for efficient data collection and dissemination in V2X networks". In First International Workshop on Smart Vehicles: Connectivity Technologies and ITS Applications (2014) (SmartVehicles'14), Sydney, Australia, June 2014.** (`PDF <https://drive.google.com/file/d/0B-Qgl9lKNlsXLWRvWllGdElHMTQ>`__)

    Named Data Networking Networking (NDN) gives more importance to content instead of its location. On the other hand, V2X networks carry a wide variety of content such as events, traffic information and infotainment content. Therefore, NDN native capabilities in terms of caching and multicast can reduce traffic load in V2X networks and consequently decrease congestion risk. However, the communication in NDN is based on a request-response pattern while it is event based in V2X. Thus, this paper extends NDN with a Pub/Sub capability in order to provide an efficient data collection and dissemination in V2X networks. In this paper, we study the limitations of using NDN and the challenges of extending it with Pub/Sub and propose a new protocol that handles V2X characteristics. Simulation results of data dissemination shows that the number of Subscribe messages generated in vehicles present at most the one tenth of the total received Publish messages (TPub) while, in NDN, the number of Interests will be equal to the number of Data messages without considering message losses.

#. **W. Drira and F. Filali, "NDN-Q: an NDN query mechanism for efficient V2X data collection in smart cities". In Self-Organizing Wireless Access Networks for Smart City (SWANSITY) part of SECON 2014 (SWANSITY 2014), Singapore, Singapore, June 2014.** (`PDF <https://drive.google.com/file/d/0B-Qgl9lKNlsXZWhwVmtxdUVUNXc>`__)

    Collecting data from V2X networks is important to monitor, control and manage road traffic. However, efficient collection of the needed data is quite challenging due to vehicles mobility and the tremendous amount of events and data generated. Named Data Networking (NDN), a future internet architecture, gives more importance to content instead of its location. It has some inner capabilities of caching and multicast that can reduce traffic load in V2X networks and consequently decrease congestion risk. The communication in NDN is based on a request-response pattern where a consumer requests a specific content using its associated name. In this paper, an NDN query mechanism is proposed to use dynamic names to collect dynamic data built on the fly in one or many nodes. Then, it is merged and aggregated hop by hop to provide the response cooperatively to the consumer.

#. **Marica Amadeo, Claudia Campolo, Antonella Molinaro, Nathalie Mitton. "Named Data
   Networking: a Natural Design for Data Collection in Wireless Sensor Networks", IFIP Wireless
   Days 2013, Valencia, Spain.** (`DOI: 10.1109/WD.2013.6686486
   <http://dx.doi.org/10.1109/WD.2013.6686486>`__, `PDF
   <https://www.academia.edu/attachments/34272952/download_file?st=MTQwNjEzNTQxOSw3OC4xNS4yMC40NSw3NjQ1Nzc%3D&>`__)

    Named Data Networking (NDN) is a promising paradigm for the future Internet architecture
    that also opens new perspectives in the way data can be retrieved in Wireless Sensor
    Networks (WSNs).  In this paper, we explore the potentialities of the NDN paradigm applied
    to WSNs and propose enhancements to the NDN forwarding strategy by including principles
    inspired by traditional data-centric routing schemes.  Results achieved through the ndnSIM
    simulator confirm the viability and effectiveness of the proposal.


#. **Marica Amadeo, Claudia Campolo, Antonella Molinaro. "Forwarding Strategies in Named Data
   Wireless Ad hoc Networks: Design and Evaluation", Elsevier Journal of Network and Computer
   Applications (JNCA), 2014.** (`DOI: 10.1016/j.jnca.2014.06.007
   <http://www.sciencedirect.com/science/article/pii/S1084804514001404>`__, `PDF
   <https://www.academia.edu/attachments/34272694/download_file?st=MTQwNjEzNTk1Miw3OC4xNS4yMC40NSw3NjQ1Nzc%3D&>`__)

    Named Data Networking (NDN) is a promising information-centric architecture for the future
    Internet that is also gaining momentum in wireless ad hoc networks as an alternative
    paradigm to traditional IP networking.  NDN shares with other information-centric proposals
    the same innovative concepts, such as named content, name-based routing, and in-network
    content caching.  These principles and the simple and robust communication model, based on
    Interest and Data packets exchange, make NDN especially appealing for deployment in
    wireless ad hoc environments, characterized by a broadcast error-prone channel and
    time-varying topologies.  Nevertheless, making NDN-based solutions really effective in ad
    hoc networks requires rethinking some of the basic NDN forwarding principles to cope with
    wireless links and node mobility.  In this paper, we analyse two classes of forwarding
    approaches: (i) a minimalist, provider-blind forwarding strategy, only aimed at keeping
    packet redundancy on the broadcast wireless medium under control, without any knowledge
    about the neighbourhood and the identity of the content sources; and (ii) a provider-aware
    strategy, which leverages soft state information about the content sources, piggybacked in
    Interest and Data packets and locally kept by nodes, to facilitate content retrieval.

    Performance evaluation is carried by means of ndnSIM, the official NDN simulator, that is
    overhauled for use in realistic wireless ad-hoc environments.  Results collected under
    variable traffic loads and topologies provide insights into the behaviour of both
    forwarding approaches and help to derive a set of recommendations that are crucial to the
    successful design of a forwarding strategy for named data ad-hoc wireless networking.

#. **S. Tarnoi, K. Suksomboon, and Y. Ji, "Cooperative Routing for Content-Centric Networking,"
   IEEE Conference on Local Computer Networks (IEEE LCN), Sydney, Australia, October 2013.**
   (`DOI:10.1109/LCN.2013.6761314 <http://dx.doi.org/10.1109/LCN.2013.6761314>`__)

    A typical Forwarding Information Based (FIB) construction in the Content Centric Networking
    (CCN) architecture relies on the name prefix dissemination following the shortest path
    manner. However, routing based on the shortest path may not fully exploit the benefits of
    forwarding and data planes of the CCN architecture since different content requester
    routers may use disjoint paths to forward their interest packets, even though these packets
    aim at the same content. To exploit this opportunity, we propose a cooperative routing
    protocol for CCN, which focuses on a FIB reconstruction based on the content retrieval
    statistics to improve the in-network caching utilization. A binary linear optimization
    problem is formulated for calculating the optimal path for the cooperative routing. The
    simulation results show an improvement in the server load and round-trip time provided by
    the cooperative routing scheme compared with that of the conventional shortest path routing
    scheme.

#. **S. Tarnoi, K. Suksomboon, W. Kumwilaisak, and Y. Ji, "Performance of probabilistic caching
   and cache replacement policies for content-centric networks", IEEE LCN, Edmonton, Canada,
   September 2014.** (`DOI:10.1109/LCN.2014.6925761
   <http://dx.doi.org/10.1109/LCN.2014.6925761>`__)

    The Content-Centric Networking (CCN) architecture exploits a universal caching strategy
    whose inefficiency has been confirmed by research communities. Various caching schemes have
    been proposed to overcome some drawbacks of the universal caching strategy but they come
    with additional complexity and overheads. Besides those sophisticated caching schemes,
    there is a probabilistic caching scheme that is more efficient than the universal caching
    strategy and adds a modest complexity to a network. The probabilistic caching scheme was
    treated as a benchmark and the insights into its behavior have never been studied despite
    its promising performance and feasibility in practical use. In this paper we study the
    probabilistic caching scheme by means of computer simulation to explore the behavior of the
    probabilistic caching scheme when it works with various cache replacement policies. The
    simulation results show the different behavioral characteristics of the probabilistic
    caching scheme as a function of the cache replacement policy.

#. **S. Tarnoi, W. Kumwilaisak, and Y. Ji, "Optimal cooperative routing protocol based on
   prefix popularity for content centric networking", IEEE LCN, Edmonton, Canada, September
   2014.** (`DOI:10.1109/LCN.2014.6925805 <http://dx.doi.org/10.1109/LCN.2014.6925805>`__)

    This paper presents an optimal cooperative routing protocol (OCRP) for Content Centric
    Networking (CCN) aiming to improve the in-network cache utilization. The objective of OCRP
    is to selectively aggregate the multiple flows of interest messages onto the same
    path. This improves the cache utilization while mitigating the cache contention in the
    Content Store (CS) of CCN routers on the routing path. The optimal routing path is obtained
    by binary linear optimization under threes constraints: flow conservation constraint, cache
    contention mitigating constraint, and path length constraint. Our simulation results of
    OCRP show the reduction in the server load and round-trip hop distance in comparison to
    those of the shortest path routing and our previously proposed cooperative routing schemes.

#. **Klaus M. Schneider, and Udo R. Krieger, "Beyond Network Selection: Exploiting Access Network
   Heterogeneity with Named Data Networking", 2nd ACM Conference on Information-Centric
   Networking, San Francisco, October, 2015** (`DOI: 10.1145/2810156.2810164 <http://dx.doi.org/10.1145/2810156.2810164>`__, `PDF <http://conferences2.sigcomm.org/acm-icn/2015/proceedings/p137-schneider.pdf>`__)

    Today, most mobile devices are equipped with multiple wireless network interfaces, but are
    constrained to use only one network at the same time. In this paper, we show that using multiple
    access networks simultaneously can improve user-perceived QoS and cost- effectiveness. We
    present a system architecture that exploits the adap- tive forwarding plane of Named Data
    Networking (CCN/NDN) and implement a system prototype based on the NDN Forwarding
    Daemon (NFD). More specifically, we propose a set of forwarding strate- gies that use
    fine-grained application requirements together with interfaces estimation techniques for delay,
    bandwidth, and packet loss. Our simulation results show that our approach can improve QoS
    and/or reduce access costs in many wireless scenarios.
