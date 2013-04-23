ndnSIM research papers
======================

Here is a list of ndnSIM-related papers.  If you want your paper to appear in this list, please send an email to our mailing list or to us directly.

General papers about ndnSIM
---------------------------

.. note::
    if you refer to ndnSIM in a published work, please cite this paper, not just the ndnSIM website

- **A. Afanasyev, I. Moiseenko, and L. Zhang, "ndnSIM: NDN simulator for NS-3," NDN, Technical Report NDN-0005, 2012** (`PDF <http://named-data.net/techreport/TR005-ndnsim.pdf>`_, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/367>`_)
    Named Data Networking (NDN) is a newly proposed Internet architecture.
    NDN retains the Internet's hourglass architecture but evolves the thin waist. 
    Instead of pushing data to specific locations, NDN retrieves data by name.  
    On one hand, this simple change allows NDN networks to use almost all of the Internet's well tested engineering properties to solve not only IP's communication problems but also digital distribution and control problems.  
    On the other hand, a distribution architecture differs in fundamental ways from a point-to-point communication architecture of today's Internet and raises many new research challenges.  
    Simulation can serve as a flexible tool to examine and evaluate various aspects of this new architecture. 
    To provide the research community at large with a common simulation platform, we have developed an open source NS-3 based simulator, ndnSIM, which faithfully implemented the basic components of a NDN network in a  modular way. 
    This paper provides an overview of ndnSIM.
    

Research papers that use ndnSIM
-------------------------------

- **B. Zhou, C. Wu, X. Hong, and M. Jiang, "Algorithms for Distributed Programmable Controllers", Technical Report, March 2013.** (`PDF <http://hong.cs.ua.edu/DCP-techReport-March2013.pdf>`_)
    A few works on SDN (Software-Deﬁned Networking) like those in Onix improve programmability of the distributed network control. The asynchronism and Byzantine issues of the control challenge the re-conﬁgurability of the service that is to safely program the control in atomic so as to avoid the transient control issues like the routing loops and black holes. We propose two important algorithms of the distributed control to enable the programmability: (1) the reconﬁguration primitive allows the network control of the services being able to safely react to an external event; and (2) the reuse primitive allows the control states of a service being accessible for all services. We give concepts and algorithms of two primitives. In addition, we provide the concrete cases of the current approaches for ICN (Information-Centric Networking) and CDN (Content Distribution Networks) for quests of the reconﬁgurability and programmability. Then, we evaluate the performance of ICN in both simulation and the PlanetLab testbed. The evaluation results show that the layer improves the lowers 19.6% of the Interest delays in the ICN that is heavily congested and lowers 97% delays in the PlanetLab with 9 nodes on usual case. In addition, the evaluation of CDN on the PlanetLab shows that it reduces 81% request delay on usual case.

- **A. Afanasyev, P. Mahadevan, I. Moiseenko, E. Uzun, and L. Zhang, "Interest Flooding Attack and Countermeasures in Named Data Networking," in Proc. of IFIP Networking 2013, May 2013.** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ifip-interest-flooding-ndn.pdf>`_, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/381>`_, `simulation code <https://github.com/cawka/ndnSIM-ddos-interest-flooding>`_)
    Distributed Denial of Service (DDoS) attacks are an ongoing problem in today's Internet, where packets from a large number of compromised hosts thwart the paths to the victim site and/or overload the victim machines. 
    In a newly proposed future Internet architecture, Named Data Networking (NDN), end users request desired data by sending Interest packets, and the network delivers Data packets upon request only, effectively eliminating many existing DDoS attacks. 
    However, an NDN network can be subject to a new type of DDoS attack, namely Interest packet flooding.  
    In this paper we investigate effective solutions to mitigate Interest flooding.
    We show that NDN's inherent properties of storing per packet state on each router and maintaining flow balance (i.e., one Interest packet retrieves at most one Data packet) provides the  basis for effective DDoS mitigation algorithms.
    Our evaluation through simulations shows that the solution can quickly and effectively respond and mitigate Interest flooding.
    
- **C. Yi, A. Afanasyev, I. Moiseenko, L. Wang, B. Zhang, and L. Zhang, "A Case for Stateful Forwarding Plane," Computer Communications, vol. 36, no. 7, pp. 779–791, 2013. ISSN 0140-3664** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Yi/comcom-stateful-forwarding.pdf>`_, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/380>`_, `simulation code <https://github.com/cawka/ndnSIM-comcom-stateful-fw>`_)
    In Named Data Networking (NDN), packets carry data names instead of source and destination addresses. 
    This paradigm shift leads to a new network forwarding plane: data consumers send *Interest* packets to request desired data, routers forward Interest packets and maintain the state of all pending Interests, which is then used to guide *Data* packets back to the consumers.
    Maintaining the pending Interest state, together with the two-way Interest and Data exchange, enables NDN routers' *forwarding* process to measure performance of different paths, quickly detect failures and retry alternative paths.
    In this paper we describe an initial design of NDN's forwarding plane and evaluate its data delivery performance under adverse conditions.
    Our results show that this stateful forwarding plane can successfully circumvent prefix hijackers, avoid failed links, and utilize multiple paths to mitigate congestion.  
    We also compare NDN's performance with that of IP-based solutions to highlight the advantages of a stateful forwarding plane.

- **M. Vahlenkamp, "Threats on Information-Centric Networking", Hamburg University of Applied Sciences, Technical Report, 2012.** (`PDF <http://inet.cpt.haw-hamburg.de/teaching/ws-2012-13/master-projekt/markus-vahlenkamp_seminar.pdf>`_)
    The ICN approach aims for reﬂecting these changes in usage of the Internet and is thus dragging content awareness into the network, for instance to let the network itself decide where to acquire requested data from and thereby utilize content caches to increase the data dissemination efﬁciency. All this is backed by the use of the publish/subscribe paradigm that is utilised to announce content availability and request its delivery. Since the ICN paradigm is entirely different from todays Internet, new challenges arise within the area of network security. NDN/CCNx, as the most popular ICN approach, claims to solve a couple of different security ﬂaws which the actual Internet is suffering from. This raises the questions of which vulnerabilities still exist and if maybe new issues arise.

- **Z. Zhu, C. Bian, A. Afanasyev, V. Jacobson, and L. Zhang, "Chronos: Serverless Multi-User Chat Over NDN," NDN, Technical Report NDN-0008, 2012.** (`PDF <http://named-data.net/techreport/TR008-chronos.pdf>`_, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/371>`_) 
    Multi-user applications are commonly implemented using a centralized server.
    This paper presents a new design for multi-user chat applications (Chronos) that works in a distributed, serverless fashion over Named Data Networking.
    In Chronos, all participants share their views by exchanging the cryptographic digests of the chat room data set.
    A newly generated message causes a change of the digest at the message originator, which leads to retrieving the new data by all other participants in an efficient way and resynchronization of chat room views.
    Chronos does not have a single point of failure and eliminates traffic concentration problem of server-based implementations.
    We use simulations to evaluate and compare Chronos with a traditional server-based chat room implementation.
    Our results demonstrate Chronos' robustness and efficiency in data dissemination.
    Chronos' approach of replacing centralized servers by distributed data synchronization can be applied to a variety of distributed applications to simplify design and ease deployment.

- **L. Wang, A. Afanasyev, R. Kuntz, R. Vuyyuru, R. Wakikawa, and L. Zhang, "Rapid Traffic Information Dissemination Using Named Data," in Proceedings of the 1st ACM workshop on Emerging Name-Oriented Mobile Networking Design - Architecture, Algorithms, and Applications (NoM'12), Hilton Head Island, South Carolina, June 2012, pp. 7–12.** (`PDF <http://lasr.cs.ucla.edu/afanasyev/data/files/Wang/nom.pdf>`_, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/365>`_, `simulation code <https://github.com/cawka/ndnSIM-nom-rapid-car2car>`_)
    Our previous work applied the Named Data Networking approach to vehicle-to-vehicle (V2V) communications and developed a simple design for traffic information dissemination applications. This paper uses simulations to evaluate the feasibility of the design as described in [1]. 
    Our results show that data names can greatly facilitate the forwarding process for Interest and data packets. 
    With adequate vehicle density, data can propagate over long distances robustly at tens of kilometers per second and a requester can retrieve the desired traffic information 10km away in a matter of seconds.
