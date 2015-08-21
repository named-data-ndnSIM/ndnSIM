#!/bin/bash

size=100
rate=1000
sim_time=$(( 2000 / rate ))

# scenarios using the ndnSIM CS
echo "Using ndnSIM's CS.."

# scenarios using the Lru replacement policy
echo "Lru replacement policy.."

# scenarios for the evaluation of memory overhead per PIT entry
echo "Evaluation of memory overhead per PIT entry.."

echo "CS size = " $size, "interest rate = " $rate

# using multicast forwarding strategy
echo "Using multicast forwarding strategy.."

../../../waf --run ndn-test --command-template="%s --old-cs=ns3::ndn::cs::Lru --cs-size=${size} --rate=${rate} --sim-time=${sim_time} --pit=$(true) --strategy="/localhost/nfd/strategy/multicast" "

echo

# using best route forwarding strategy
echo "Using best route forwarding strategy.."

../../../waf --run ndn-test --command-template="%s --old-cs=ns3::ndn::cs::Lru --cs-size=${size} --rate=${rate} --pit=$(true) --strategy="/localhost/nfd/strategy/best-route" --sim-time=${sim_time}"

echo

size=100000
rate=100
sim_time=$(( 2000 / rate ))

# scenarios using the NFD's CS
echo "Using NFD's CS.."

# scenarios for the evaluation of memory overhead per CS entry
echo "Evaluation of memory overhead per CS entry.."

echo "CS size = " $size, "interest rate = " $rate

# using multicast forwarding strategy
echo "Using multicast forwarding strategy.."

../../../waf --run ndn-test --command-template="%s --cs-size=${size} --rate=${rate} --strategy="/localhost/nfd/strategy/multicast" --sim-time=${sim_time}"

echo

# using best route forwarding strategy
echo "Using best route forwarding strategy.."

../../../waf --run ndn-test --command-template="%s --cs-size=${size} --rate=${rate} --strategy="/localhost/nfd/strategy/best-route" --sim-time=${sim_time}"
