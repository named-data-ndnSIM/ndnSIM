/*!

\class ns3::ndn::App

<h3>Config Paths</h3>

ns3::ndn::App is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App</li>
</ul>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::Consumer

<h3>Config Paths</h3>

ns3::ndn::Consumer is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>StartSeq</b>: Initial sequence number
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref int32_t -2147483648:2147483647</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Prefix</b>: Name of the Interest
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>LifeTime</b>: LifeTime for interest packet
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 2s</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 50ms</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data
</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::ConsumerBatches

<h3>Config Paths</h3>

ns3::ndn::ConsumerBatches is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerBatches</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%ConsumerBatches</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerBatches</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%ConsumerBatches</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>Batches</b>: Batches to schedule. Should be vector, containing pairs of time and amount
  <ul>
    <li>Set with class: \ref BatchesValue</li>
    <li>Underlying type: \ref Batches</li>
    <li>Initial value: </li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>StartSeq</b>: Initial sequence number
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref int32_t -2147483648:2147483647</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Prefix</b>: Name of the Interest
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>LifeTime</b>: LifeTime for interest packet
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 2s</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 50ms</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data
</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::ConsumerCbr

<h3>Config Paths</h3>

ns3::ndn::ConsumerCbr is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerCbr</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%ConsumerCbr</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerCbr</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%ConsumerCbr</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>Frequency</b>: Frequency of interest packets
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1.0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Randomize</b>: Type of send time randomization: none (default), uniform, exponential
  <ul>
    <li>Set with class: \ref StringValue</li>
    <li>Underlying type: </li>
    <li>Initial value: none</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>MaxSeq</b>: Maximum sequence number to request
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 4294967295</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>StartSeq</b>: Initial sequence number
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref int32_t -2147483648:2147483647</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Prefix</b>: Name of the Interest
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>LifeTime</b>: LifeTime for interest packet
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 2s</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 50ms</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data
</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::ConsumerWindow

<h3>Config Paths</h3>

ns3::ndn::ConsumerWindow is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerWindow</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%ConsumerWindow</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerWindow</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%ConsumerWindow</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>Window</b>: Initial size of the window
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>PayloadSize</b>: Average size of content object size (to calculate interest generation rate)
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 1040</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Size</b>: Amount of data in megabytes to request, relying on PayloadSize parameter (alternative to MaxSeq attribute)
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: -1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>MaxSeq</b>: Maximum sequence number to request (alternative to Size attribute, would activate only if Size is -1). The parameter is activated only if Size negative (not set)
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 4294967295</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>InitialWindowOnTimeout</b>: Set window to initial value when timeout occurs
  <ul>
    <li>Set with class: \ref BooleanValue</li>
    <li>Underlying type: \ref bool</li>
    <li>Initial value: true</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>StartSeq</b>: Initial sequence number
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref int32_t -2147483648:2147483647</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Prefix</b>: Name of the Interest
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>LifeTime</b>: LifeTime for interest packet
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 2s</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 50ms</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>WindowTrace</b>: Window that controls how many outstanding interests are allowed
</li>
<li><b>InFlight</b>: Current number of outstanding interests
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data
</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::ConsumerZipfMandelbrot

<h3>Config Paths</h3>

ns3::ndn::ConsumerZipfMandelbrot is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerCbr/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%ConsumerCbr/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerCbr/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Consumer/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%ConsumerCbr/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%ConsumerZipfMandelbrot</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>NumberOfContents</b>: Number of the Contents in total
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>q</b>: parameter of improve rank
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 0.7</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>s</b>: parameter of power
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 0.7</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::ConsumerCbr</h3>
<ul>
<li><b>Frequency</b>: Frequency of interest packets
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1.0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Randomize</b>: Type of send time randomization: none (default), uniform, exponential
  <ul>
    <li>Set with class: \ref StringValue</li>
    <li>Underlying type: </li>
    <li>Initial value: none</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>MaxSeq</b>: Maximum sequence number to request
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 4294967295</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>StartSeq</b>: Initial sequence number
  <ul>
    <li>Set with class: \ref ns3::IntegerValue</li>
    <li>Underlying type: \ref int32_t -2147483648:2147483647</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Prefix</b>: Name of the Interest
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>LifeTime</b>: LifeTime for interest packet
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 2s</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: 50ms</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::Consumer</h3>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data
</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!

\class ns3::ndn::ContentStore

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Fifo

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Fifo::AggregateStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Fifo::Freshness

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Fifo::LifetimeStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Freshness::Fifo

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::Fifo::Freshness</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Freshness::Lfu

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::Lfu::Freshness</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Freshness::Lru

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::Lru::Freshness</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Freshness::Random

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::Random::Freshness</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lfu

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lfu::AggregateStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lfu::Freshness

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lfu::LifetimeStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lru

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lru::AggregateStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lru::Freshness

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Lru::LifetimeStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Nocache

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Probability::Fifo

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::cs::ProbabilityImpl::Fifo</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::ProbabilityImpl::Fifo</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Probability::Lfu

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::cs::ProbabilityImpl::Lfu</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::ProbabilityImpl::Lfu</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Probability::Lru

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::cs::ProbabilityImpl::Lru</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::ProbabilityImpl::Lru</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Probability::Random

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.
  <ul>
    <li>Set with class: \ref ns3::DoubleValue</li>
    <li>Underlying type: \ref double -1.79769e+308:1.79769e+308</li>
    <li>Initial value: 1</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::ndn::cs::ProbabilityImpl::Random</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::cs::ProbabilityImpl::Random</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::ProbabilityImpl::Fifo

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::ProbabilityImpl::Lfu

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::ProbabilityImpl::Lru

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::ProbabilityImpl::Random

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Random

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Random::AggregateStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Random::Freshness

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Random::LifetimeStats

Doxygen introspection did not find any typical Config paths.<br>
<h3>Attributes</h3>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 100</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>TraceSources</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Stats::Fifo

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::cs::Fifo::LifetimeStats</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Stats::Lfu

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::cs::Lfu::LifetimeStats</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Stats::Lru

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::cs::Lru::LifetimeStats</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::cs::Stats::Random

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
<h3>TraceSources</h3>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::cs::Random::LifetimeStats</h3>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache
</li>
</ul>
<h3>TraceSources defined in parent class ns3::ndn::ContentStore</h3>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit
</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss
</li>
</ul>
*/

/*!

\class ns3::ndn::Data

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
*/

/*!

\class ns3::ndn::GlobalRouter

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
*/

/*!

\class ns3::ndn::Interest

Doxygen introspection did not find any typical Config paths.<br>
No Attributes are defined for this type.<br>
No TraceSources are defined for this type.<br>
*/

/*!

\class ns3::ndn::Producer

<h3>Config Paths</h3>

ns3::ndn::Producer is accessible through the following paths with Config::Set and Config::Connect:
<ul>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%App/$%ns3::%ndn::%Producer</li>
<li>/NodeList/[i]/ApplicationList/[i]/$%ns3::%ndn::%Producer</li>
</ul>
<h3>Attributes</h3>
<ul>
<li><b>Prefix</b>: Prefix, for which producer has the data
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Postfix</b>: Postfix that is added to the output data (e.g., for adding producer-uniqueness)
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>PayloadSize</b>: Virtual payload size for Content packets
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 1024</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Freshness</b>: Freshness of data packets, if 0, then unlimited freshness
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>Signature</b>: Fake signature, 0 valid signature (default), other values application-specific
  <ul>
    <li>Set with class: \ref ns3::UintegerValue</li>
    <li>Underlying type: \ref uint32_t 0:4294967295</li>
    <li>Initial value: 0</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>KeyLocator</b>: Name to be used for key locator.  If root, then key locator is not used
  <ul>
    <li>Set with class: \ref NameValue</li>
    <li>Underlying type: \ref Name</li>
    <li>Initial value: /</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
<h3>Attributes defined in parent class ns3::Application</h3>
<ul>
<li><b>StartTime</b>: Time at which the application will start
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
<li><b>StopTime</b>: Time at which the application will stop
  <ul>
    <li>Set with class: \ref ns3::TimeValue</li>
    <li>Underlying type: \ref Time --9223372036854775808.0ns:+9223372036854775807.0ns</li>
    <li>Initial value: +0.0ns</li>
    <li>Flags: <span class="mlabel">construct </span><span class="mlabel">write </span><span class="mlabel">read </span></li>
  </ul> 
</ul>
No TraceSources are defined for this type.<br>
<h3>TraceSources defined in parent class ns3::ndn::App</h3>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests
</li>
<li><b>ReceivedDatas</b>: ReceivedDatas
</li>
<li><b>TransmittedInterests</b>: TransmittedInterests
</li>
<li><b>TransmittedDatas</b>: TransmittedDatas
</li>
</ul>
*/

/*!
\page TraceSourceList All TraceSources

<b>ns3::ndn::App</b><br>
<ul>
<li><b>ReceivedInterests</b>: ReceivedInterests</li>
<li><b>ReceivedDatas</b>: ReceivedDatas</li>
<li><b>TransmittedInterests</b>: TransmittedInterests</li>
<li><b>TransmittedDatas</b>: TransmittedDatas</li>
</ul>
<b>ns3::ndn::Consumer</b><br>
<ul>
<li><b>LastRetransmittedInterestDataDelay</b>: Delay between last retransmitted Interest and received Data</li>
<li><b>FirstInterestDataDelay</b>: Delay between first transmitted Interest and received Data</li>
</ul>
<b>ns3::ndn::ConsumerWindow</b><br>
<ul>
<li><b>WindowTrace</b>: Window that controls how many outstanding interests are allowed</li>
<li><b>InFlight</b>: Current number of outstanding interests</li>
</ul>
<b>ns3::ndn::cs::Lru</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::ContentStore</b><br>
<ul>
<li><b>CacheHits</b>: Trace called every time there is a cache hit</li>
<li><b>CacheMisses</b>: Trace called every time there is a cache miss</li>
</ul>
<b>ns3::ndn::cs::Random</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Fifo</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Lfu</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Lru::AggregateStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Random::AggregateStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Fifo::AggregateStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Lfu::AggregateStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Lru::Freshness</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Random::Freshness</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Fifo::Freshness</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Lfu::Freshness</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Lru</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Random</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Fifo</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Lfu</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Stats::Lru</b><br>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed</li>
</ul>
<b>ns3::ndn::cs::Lru::LifetimeStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Stats::Random</b><br>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed</li>
</ul>
<b>ns3::ndn::cs::Random::LifetimeStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Stats::Fifo</b><br>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed</li>
</ul>
<b>ns3::ndn::cs::Fifo::LifetimeStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
<b>ns3::ndn::cs::Stats::Lfu</b><br>
<ul>
<li><b>WillRemoveEntry</b>: Trace called just before content store entry will be removed</li>
</ul>
<b>ns3::ndn::cs::Lfu::LifetimeStats</b><br>
<ul>
<li><b>DidAddEntry</b>: Trace fired every time entry is successfully added to the cache</li>
</ul>
*/

/*!
\page AttributesList All Attributes

<b>ns3::RandomVariableStream</b><br>
<ul>
<li><b>Stream</b>: The stream number for this RNG stream. -1 means "allocate a stream automatically". Note that if -1 is set, Get will return -1 so that it is not possible to know which value was automatically allocated.</li>
<li><b>Antithetic</b>: Set this RNG stream to generate antithetic values</li>
</ul>
<b>ns3::UniformRandomVariable</b><br>
<ul>
<li><b>Min</b>: The lower bound on the values returned by this RNG stream.</li>
<li><b>Max</b>: The upper bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::ConstantRandomVariable</b><br>
<ul>
<li><b>Constant</b>: The constant value returned by this RNG stream.</li>
</ul>
<b>ns3::SequentialRandomVariable</b><br>
<ul>
<li><b>Min</b>: The first value of the sequence.</li>
<li><b>Max</b>: One more than the last value of the sequence.</li>
<li><b>Increment</b>: The sequence random variable increment.</li>
<li><b>Consecutive</b>: The number of times each member of the sequence is repeated.</li>
</ul>
<b>ns3::ExponentialRandomVariable</b><br>
<ul>
<li><b>Mean</b>: The mean of the values returned by this RNG stream.</li>
<li><b>Bound</b>: The upper bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::ParetoRandomVariable</b><br>
<ul>
<li><b>Mean</b>: The mean parameter for the Pareto distribution returned by this RNG stream.</li>
<li><b>Shape</b>: The shape parameter for the Pareto distribution returned by this RNG stream.</li>
<li><b>Bound</b>: The upper bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::WeibullRandomVariable</b><br>
<ul>
<li><b>Scale</b>: The scale parameter for the Weibull distribution returned by this RNG stream.</li>
<li><b>Shape</b>: The shape parameter for the Weibull distribution returned by this RNG stream.</li>
<li><b>Bound</b>: The upper bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::NormalRandomVariable</b><br>
<ul>
<li><b>Mean</b>: The mean value for the normal distribution returned by this RNG stream.</li>
<li><b>Variance</b>: The variance value for the normal distribution returned by this RNG stream.</li>
<li><b>Bound</b>: The bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::LogNormalRandomVariable</b><br>
<ul>
<li><b>Mu</b>: The mu value for the log-normal distribution returned by this RNG stream.</li>
<li><b>Sigma</b>: The sigma value for the log-normal distribution returned by this RNG stream.</li>
</ul>
<b>ns3::GammaRandomVariable</b><br>
<ul>
<li><b>Alpha</b>: The alpha value for the gamma distribution returned by this RNG stream.</li>
<li><b>Beta</b>: The beta value for the gamma distribution returned by this RNG stream.</li>
</ul>
<b>ns3::ErlangRandomVariable</b><br>
<ul>
<li><b>K</b>: The k value for the Erlang distribution returned by this RNG stream.</li>
<li><b>Lambda</b>: The lambda value for the Erlang distribution returned by this RNG stream.</li>
</ul>
<b>ns3::TriangularRandomVariable</b><br>
<ul>
<li><b>Mean</b>: The mean value for the triangular distribution returned by this RNG stream.</li>
<li><b>Min</b>: The lower bound on the values returned by this RNG stream.</li>
<li><b>Max</b>: The upper bound on the values returned by this RNG stream.</li>
</ul>
<b>ns3::ZipfRandomVariable</b><br>
<ul>
<li><b>N</b>: The n value for the Zipf distribution returned by this RNG stream.</li>
<li><b>Alpha</b>: The alpha value for the Zipf distribution returned by this RNG stream.</li>
</ul>
<b>ns3::ZetaRandomVariable</b><br>
<ul>
<li><b>Alpha</b>: The alpha value for the zeta distribution returned by this RNG stream.</li>
</ul>
<b>ns3::DataCollectionObject</b><br>
<ul>
<li><b>Name</b>: Object's name</li>
<li><b>Enabled</b>: Object's enabled status</li>
</ul>
<b>ns3::Probe</b><br>
<ul>
<li><b>Start</b>: Time data collection starts</li>
<li><b>Stop</b>: Time when data collection stops.  The special time value of 0 disables this attribute</li>
</ul>
<b>ns3::Application</b><br>
<ul>
<li><b>StartTime</b>: Time at which the application will start</li>
<li><b>StopTime</b>: Time at which the application will stop</li>
</ul>
<b>ns3::Channel</b><br>
<ul>
<li><b>Id</b>: The id (unique integer) of this Channel.</li>
</ul>
<b>ns3::ChannelListPriv</b><br>
<ul>
<li><b>ChannelList</b>: The list of all channels created during the simulation.</li>
</ul>
<b>ns3::Node</b><br>
<ul>
<li><b>DeviceList</b>: The list of devices associated to this Node.</li>
<li><b>ApplicationList</b>: The list of applications associated to this Node.</li>
<li><b>Id</b>: The id (unique integer) of this Node.</li>
<li><b>SystemId</b>: The systemId of this node: a unique integer used for parallel simulations.</li>
</ul>
<b>ns3::NodeListPriv</b><br>
<ul>
<li><b>NodeList</b>: The list of all nodes created during the simulation.</li>
</ul>
<b>ns3::DropTailQueue</b><br>
<ul>
<li><b>Mode</b>: Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.</li>
<li><b>MaxPackets</b>: The maximum number of packets accepted by this DropTailQueue.</li>
<li><b>MaxBytes</b>: The maximum number of bytes accepted by this DropTailQueue.</li>
</ul>
<b>ns3::ErrorModel</b><br>
<ul>
<li><b>IsEnabled</b>: Whether this ErrorModel is enabled or not.</li>
</ul>
<b>ns3::RateErrorModel</b><br>
<ul>
<li><b>ErrorUnit</b>: The error unit</li>
<li><b>ErrorRate</b>: The error rate.</li>
<li><b>RanVar</b>: The decision variable attached to this error model.</li>
</ul>
<b>ns3::BurstErrorModel</b><br>
<ul>
<li><b>ErrorRate</b>: The burst error event.</li>
<li><b>BurstStart</b>: The decision variable attached to this error model.</li>
<li><b>BurstSize</b>: The number of packets being corrupted at one drop.</li>
</ul>
<b>ns3::PacketSocket</b><br>
<ul>
<li><b>RcvBufSize</b>: PacketSocket maximum receive buffer size (bytes)</li>
</ul>
<b>ns3::PcapFileWrapper</b><br>
<ul>
<li><b>CaptureSize</b>: Maximum length of captured packets (cf. pcap snaplen)</li>
</ul>
<b>ns3::RedQueue</b><br>
<ul>
<li><b>Mode</b>: Determines unit for QueueLimit</li>
<li><b>MeanPktSize</b>: Average of packet size</li>
<li><b>IdlePktSize</b>: Average packet size used during idle times. Used when m_cautions = 3</li>
<li><b>Wait</b>: True for waiting between dropped packets</li>
<li><b>Gentle</b>: True to increases dropping probability slowly when average queue exceeds maxthresh</li>
<li><b>MinTh</b>: Minimum average length threshold in packets/bytes</li>
<li><b>MaxTh</b>: Maximum average length threshold in packets/bytes</li>
<li><b>QueueLimit</b>: Queue limit in bytes/packets</li>
<li><b>QW</b>: Queue weight related to the exponential weighted moving average (EWMA)</li>
<li><b>LInterm</b>: The maximum probability of dropping a packet</li>
<li><b>Ns1Compat</b>: NS-1 compatibility</li>
<li><b>LinkBandwidth</b>: The RED link bandwidth</li>
<li><b>LinkDelay</b>: The RED link delay</li>
</ul>
<b>ns3::SimpleChannel</b><br>
<ul>
<li><b>Delay</b>: Transmission delay through the channel</li>
</ul>
<b>ns3::SimpleNetDevice</b><br>
<ul>
<li><b>ReceiveErrorModel</b>: The receiver error model used to simulate packet loss</li>
<li><b>PointToPointMode</b>: The device is configured in Point to Point mode</li>
<li><b>TxQueue</b>: A queue to use as the transmit queue in the device.</li>
<li><b>DataRate</b>: The default data rate for point to point links. Zero means infinite</li>
</ul>
<b>ns3::PacketSocketClient</b><br>
<ul>
<li><b>MaxPackets</b>: The maximum number of packets the application will send (zero means infinite)</li>
<li><b>Interval</b>: The time to wait between packets</li>
<li><b>PacketSize</b>: Size of packets generated (bytes).</li>
</ul>
<b>ns3::MobilityModel</b><br>
<ul>
<li><b>Position</b>: The current position of the mobility model.</li>
<li><b>Velocity</b>: The current velocity of the mobility model.</li>
</ul>
<b>ns3::GaussMarkovMobilityModel</b><br>
<ul>
<li><b>Bounds</b>: Bounds of the area to cruise.</li>
<li><b>TimeStep</b>: Change current direction and speed after moving for this time.</li>
<li><b>Alpha</b>: A constant representing the tunable parameter in the Gauss-Markov model.</li>
<li><b>MeanVelocity</b>: A random variable used to assign the average velocity.</li>
<li><b>MeanDirection</b>: A random variable used to assign the average direction.</li>
<li><b>MeanPitch</b>: A random variable used to assign the average pitch.</li>
<li><b>NormalVelocity</b>: A gaussian random variable used to calculate the next velocity value.</li>
<li><b>NormalDirection</b>: A gaussian random variable used to calculate the next direction value.</li>
<li><b>NormalPitch</b>: A gaussian random variable used to calculate the next pitch value.</li>
</ul>
<b>ns3::HierarchicalMobilityModel</b><br>
<ul>
<li><b>Child</b>: The child mobility model.</li>
<li><b>Parent</b>: The parent mobility model.</li>
</ul>
<b>ns3::GridPositionAllocator</b><br>
<ul>
<li><b>GridWidth</b>: The number of objects layed out on a line.</li>
<li><b>MinX</b>: The x coordinate where the grid starts.</li>
<li><b>MinY</b>: The y coordinate where the grid starts.</li>
<li><b>DeltaX</b>: The x space between objects.</li>
<li><b>DeltaY</b>: The y space between objects.</li>
<li><b>LayoutType</b>: The type of layout.</li>
</ul>
<b>ns3::RandomRectanglePositionAllocator</b><br>
<ul>
<li><b>X</b>: A random variable which represents the x coordinate of a position in a random rectangle.</li>
<li><b>Y</b>: A random variable which represents the y coordinate of a position in a random rectangle.</li>
</ul>
<b>ns3::RandomBoxPositionAllocator</b><br>
<ul>
<li><b>X</b>: A random variable which represents the x coordinate of a position in a random box.</li>
<li><b>Y</b>: A random variable which represents the y coordinate of a position in a random box.</li>
<li><b>Z</b>: A random variable which represents the z coordinate of a position in a random box.</li>
</ul>
<b>ns3::RandomDiscPositionAllocator</b><br>
<ul>
<li><b>Theta</b>: A random variable which represents the angle (gradients) of a position in a random disc.</li>
<li><b>Rho</b>: A random variable which represents the radius of a position in a random disc.</li>
<li><b>X</b>: The x coordinate of the center of the random position disc.</li>
<li><b>Y</b>: The y coordinate of the center of the random position disc.</li>
</ul>
<b>ns3::UniformDiscPositionAllocator</b><br>
<ul>
<li><b>rho</b>: The radius of the disc</li>
<li><b>X</b>: The x coordinate of the center of the  disc.</li>
<li><b>Y</b>: The y coordinate of the center of the  disc.</li>
</ul>
<b>ns3::RandomDirection2dMobilityModel</b><br>
<ul>
<li><b>Bounds</b>: The 2d bounding area</li>
<li><b>Speed</b>: A random variable to control the speed (m/s).</li>
<li><b>Pause</b>: A random variable to control the pause (s).</li>
</ul>
<b>ns3::RandomWalk2dMobilityModel</b><br>
<ul>
<li><b>Bounds</b>: Bounds of the area to cruise.</li>
<li><b>Time</b>: Change current direction and speed after moving for this delay.</li>
<li><b>Distance</b>: Change current direction and speed after moving for this distance.</li>
<li><b>Mode</b>: The mode indicates the condition used to change the current speed and direction</li>
<li><b>Direction</b>: A random variable used to pick the direction (gradients).</li>
<li><b>Speed</b>: A random variable used to pick the speed (m/s).</li>
</ul>
<b>ns3::RandomWaypointMobilityModel</b><br>
<ul>
<li><b>Speed</b>: A random variable used to pick the speed of a random waypoint model.</li>
<li><b>Pause</b>: A random variable used to pick the pause of a random waypoint model.</li>
<li><b>PositionAllocator</b>: The position model used to pick a destination point.</li>
</ul>
<b>ns3::SteadyStateRandomWaypointMobilityModel</b><br>
<ul>
<li><b>MinSpeed</b>: Minimum speed value, [m/s]</li>
<li><b>MaxSpeed</b>: Maximum speed value, [m/s]</li>
<li><b>MinPause</b>: Minimum pause value, [s]</li>
<li><b>MaxPause</b>: Maximum pause value, [s]</li>
<li><b>MinX</b>: Minimum X value of traveling region, [m]</li>
<li><b>MaxX</b>: Maximum X value of traveling region, [m]</li>
<li><b>MinY</b>: Minimum Y value of traveling region, [m]</li>
<li><b>MaxY</b>: Maximum Y value of traveling region, [m]</li>
<li><b>Z</b>: Z value of traveling region (fixed), [m]</li>
</ul>
<b>ns3::WaypointMobilityModel</b><br>
<ul>
<li><b>NextWaypoint</b>: The next waypoint used to determine position.</li>
<li><b>WaypointsLeft</b>: The number of waypoints remaining.</li>
<li><b>LazyNotify</b>: Only call NotifyCourseChange when position is calculated.</li>
<li><b>InitialPositionIsWaypoint</b>: Calling SetPosition with no waypoints creates a waypoint.</li>
</ul>
<b>ns3::RandomPropagationDelayModel</b><br>
<ul>
<li><b>Variable</b>: The random variable which generates random delays (s).</li>
</ul>
<b>ns3::ConstantSpeedPropagationDelayModel</b><br>
<ul>
<li><b>Speed</b>: The speed (m/s)</li>
</ul>
<b>ns3::RandomPropagationLossModel</b><br>
<ul>
<li><b>Variable</b>: The random variable used to pick a loss everytime CalcRxPower is invoked.</li>
</ul>
<b>ns3::FriisPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The carrier frequency (in Hz) at which propagation occurs  (default is 5.15 GHz).</li>
<li><b>SystemLoss</b>: The system loss</li>
<li><b>MinLoss</b>: The minimum value (dB) of the total loss, used at short ranges. Note: </li>
</ul>
<b>ns3::TwoRayGroundPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The carrier frequency (in Hz) at which propagation occurs  (default is 5.15 GHz).</li>
<li><b>SystemLoss</b>: The system loss</li>
<li><b>MinDistance</b>: The distance under which the propagation model refuses to give results (m)</li>
<li><b>HeightAboveZ</b>: The height of the antenna (m) above the node's Z coordinate</li>
</ul>
<b>ns3::LogDistancePropagationLossModel</b><br>
<ul>
<li><b>Exponent</b>: The exponent of the Path Loss propagation model</li>
<li><b>ReferenceDistance</b>: The distance at which the reference loss is calculated (m)</li>
<li><b>ReferenceLoss</b>: The reference loss at reference distance (dB). (Default is Friis at 1m with 5.15 GHz)</li>
</ul>
<b>ns3::ThreeLogDistancePropagationLossModel</b><br>
<ul>
<li><b>Distance0</b>: Beginning of the first (near) distance field</li>
<li><b>Distance1</b>: Beginning of the second (middle) distance field.</li>
<li><b>Distance2</b>: Beginning of the third (far) distance field.</li>
<li><b>Exponent0</b>: The exponent for the first field.</li>
<li><b>Exponent1</b>: The exponent for the second field.</li>
<li><b>Exponent2</b>: The exponent for the third field.</li>
<li><b>ReferenceLoss</b>: The reference loss at distance d0 (dB). (Default is Friis at 1m with 5.15 GHz)</li>
</ul>
<b>ns3::NakagamiPropagationLossModel</b><br>
<ul>
<li><b>Distance1</b>: Beginning of the second distance field. Default is 80m.</li>
<li><b>Distance2</b>: Beginning of the third distance field. Default is 200m.</li>
<li><b>m0</b>: m0 for distances smaller than Distance1. Default is 1.5.</li>
<li><b>m1</b>: m1 for distances smaller than Distance2. Default is 0.75.</li>
<li><b>m2</b>: m2 for distances greater than Distance2. Default is 0.75.</li>
<li><b>ErlangRv</b>: Access to the underlying ErlangRandomVariable</li>
<li><b>GammaRv</b>: Access to the underlying GammaRandomVariable</li>
</ul>
<b>ns3::FixedRssLossModel</b><br>
<ul>
<li><b>Rss</b>: The fixed receiver Rss.</li>
</ul>
<b>ns3::MatrixPropagationLossModel</b><br>
<ul>
<li><b>DefaultLoss</b>: The default value for propagation loss, dB.</li>
</ul>
<b>ns3::RangePropagationLossModel</b><br>
<ul>
<li><b>MaxRange</b>: Maximum Transmission Range (meters)</li>
</ul>
<b>ns3::JakesProcess</b><br>
<ul>
<li><b>DopplerFrequencyHz</b>: Corresponding doppler frequency[Hz]</li>
<li><b>NumberOfOscillators</b>: The number of oscillators</li>
</ul>
<b>ns3::Cost231PropagationLossModel</b><br>
<ul>
<li><b>Lambda</b>: The wavelength  (default is 2.3 GHz at 300 000 km/s).</li>
<li><b>Frequency</b>: The Frequency  (default is 2.3 GHz).</li>
<li><b>BSAntennaHeight</b>:  BS Antenna Height (default is 50m).</li>
<li><b>SSAntennaHeight</b>:  SS Antenna Height (default is 3m).</li>
<li><b>MinDistance</b>: The distance under which the propagation model refuses to give results (m) </li>
</ul>
<b>ns3::OkumuraHataPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The propagation frequency in Hz</li>
<li><b>Environment</b>: Environment Scenario</li>
<li><b>CitySize</b>: Dimension of the city</li>
</ul>
<b>ns3::ItuR1411LosPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The propagation frequency in Hz</li>
</ul>
<b>ns3::ItuR1411NlosOverRooftopPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The Frequency  (default is 2.106 GHz).</li>
<li><b>Environment</b>: Environment Scenario</li>
<li><b>CitySize</b>: Dimension of the city</li>
<li><b>RooftopLevel</b>: The height of the rooftop level in meters</li>
<li><b>StreetsOrientation</b>: The orientation of streets in degrees [0,90] with respect to the direction of propagation</li>
<li><b>StreetsWidth</b>: The width of streets</li>
<li><b>BuildingsExtend</b>: The distance over which the buildings extend</li>
<li><b>BuildingSeparation</b>: The separation between buildings</li>
</ul>
<b>ns3::CosineAntennaModel</b><br>
<ul>
<li><b>Beamwidth</b>: The 3dB beamwidth (degrees)</li>
<li><b>Orientation</b>: The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis</li>
<li><b>MaxGain</b>: The gain (dB) at the antenna boresight (the direction of maximum gain)</li>
</ul>
<b>ns3::ParabolicAntennaModel</b><br>
<ul>
<li><b>Beamwidth</b>: The 3dB beamwidth (degrees)</li>
<li><b>Orientation</b>: The angle (degrees) that expresses the orientation of the antenna on the x-y plane relative to the x axis</li>
<li><b>MaxAttenuation</b>: The maximum attenuation (dB) of the antenna radiation pattern.</li>
</ul>
<b>ns3::ConstantSpectrumPropagationLossModel</b><br>
<ul>
<li><b>Loss</b>: Path loss (dB) between transmitter and receiver</li>
</ul>
<b>ns3::SingleModelSpectrumChannel</b><br>
<ul>
<li><b>MaxLossDb</b>: If a single-frequency PropagationLossModel is used, this value represents the maximum loss in dB for which transmissions will be passed to the receiving PHY. Signals for which the PropagationLossModel returns a loss bigger than this value will not be propagated to the receiver. This parameter is to be used to reduce the computational load by not propagating signals that are far beyond the interference range. Note that the default value corresponds to considering all signals for reception. Tune this value with care. </li>
</ul>
<b>ns3::MultiModelSpectrumChannel</b><br>
<ul>
<li><b>MaxLossDb</b>: If a single-frequency PropagationLossModel is used, this value represents the maximum loss in dB for which transmissions will be passed to the receiving PHY. Signals for which the PropagationLossModel returns a loss bigger than this value will not be propagated to the receiver. This parameter is to be used to reduce the computational load by not propagating signals that are far beyond the interference range. Note that the default value corresponds to considering all signals for reception. Tune this value with care. </li>
</ul>
<b>ns3::WaveformGenerator</b><br>
<ul>
<li><b>Period</b>: the period (=1/frequency)</li>
<li><b>DutyCycle</b>: the duty cycle of the generator, i.e., the fraction of the period that is occupied by a signal</li>
</ul>
<b>ns3::SpectrumAnalyzer</b><br>
<ul>
<li><b>Resolution</b>: the lengh of the time interval over which the power spectral density of incoming signals is averaged</li>
<li><b>NoisePowerSpectralDensity</b>: the power spectral density of the measuring instrument noise, in Watt/Hz. Mostly useful to make spectrograms look more similar to those obtained by real devices. Defaults to the value for thermal noise at 300K.</li>
</ul>
<b>ns3::AlohaNoackNetDevice</b><br>
<ul>
<li><b>Address</b>: The MAC address of this device.</li>
<li><b>Queue</b>: packets being transmitted get queued here</li>
<li><b>Mtu</b>: The Maximum Transmission Unit</li>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
</ul>
<b>ns3::HalfDuplexIdealPhy</b><br>
<ul>
<li><b>Rate</b>: The PHY rate used by this device</li>
</ul>
<b>ns3::NonCommunicatingNetDevice</b><br>
<ul>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
</ul>
<b>ns3::LrWpanMac</b><br>
<ul>
<li><b>PanId</b>: 16-bit identifier of the associated PAN</li>
</ul>
<b>ns3::LrWpanNetDevice</b><br>
<ul>
<li><b>Channel</b>: The channel attached to this device</li>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
<li><b>Mac</b>: The MAC layer attached to this device.</li>
<li><b>UseAcks</b>: Request acknowledgments for data frames.</li>
</ul>
<b>ns3::LrWpanLqiTag</b><br>
<ul>
<li><b>Lqi</b>: The lqi of the last packet received</li>
</ul>
<b>ns3::NullMessageSimulatorImpl</b><br>
<ul>
<li><b>SchedulerTune</b>: Null Message scheduler tuning parameter</li>
</ul>
<b>ns3::PointToPointNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>Address</b>: The MAC address of this device.</li>
<li><b>DataRate</b>: The default data rate for point to point links</li>
<li><b>ReceiveErrorModel</b>: The receiver error model used to simulate packet loss</li>
<li><b>InterframeGap</b>: The time to wait between packet (frame) transmissions</li>
<li><b>TxQueue</b>: A queue to use as the transmit queue in the device.</li>
</ul>
<b>ns3::PointToPointChannel</b><br>
<ul>
<li><b>Delay</b>: Transmission delay through the channel</li>
</ul>
<b>ns3::BridgeNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>EnableLearning</b>: Enable the learning mode of the Learning Bridge</li>
<li><b>ExpirationTime</b>: Time it takes for learned MAC state entry to expire.</li>
</ul>
<b>ns3::IpL4Protocol</b><br>
<ul>
<li><b>ProtocolNumber</b>: The Ip protocol number.</li>
</ul>
<b>ns3::Ipv4Interface</b><br>
<ul>
<li><b>ArpCache</b>: The arp cache for this ipv4 interface</li>
</ul>
<b>ns3::Ipv4L3Protocol</b><br>
<ul>
<li><b>DefaultTos</b>: The TOS value set by default on all outgoing packets generated on this node.</li>
<li><b>DefaultTtl</b>: The TTL value set by default on all outgoing packets generated on this node.</li>
<li><b>FragmentExpirationTimeout</b>: When this timeout expires, the fragments will be cleared from the buffer.</li>
<li><b>InterfaceList</b>: The set of Ipv4 interfaces associated to this Ipv4 stack.</li>
</ul>
<b>ns3::Ipv4</b><br>
<ul>
<li><b>IpForward</b>: Globally enable or disable IP forwarding for all current and future Ipv4 devices.</li>
<li><b>WeakEsModel</b>: RFC1122 term for whether host accepts datagram with a dest. address on another interface</li>
</ul>
<b>ns3::UdpL4Protocol</b><br>
<ul>
<li><b>SocketList</b>: The list of sockets associated to this protocol.</li>
</ul>
<b>ns3::TcpL4Protocol</b><br>
<ul>
<li><b>RttEstimatorType</b>: Type of RttEstimator objects.</li>
<li><b>SocketType</b>: Socket type of TCP objects.</li>
<li><b>SocketList</b>: The list of sockets associated to this protocol.</li>
</ul>
<b>ns3::RttMeanDeviation</b><br>
<ul>
<li><b>Gain</b>: Gain used in estimating the RTT, must be 0 < Gain < 1</li>
</ul>
<b>ns3::RttEstimator</b><br>
<ul>
<li><b>MaxMultiplier</b>: Maximum RTO Multiplier</li>
<li><b>InitialEstimation</b>: Initial RTT estimation</li>
<li><b>MinRTO</b>: Minimum retransmit timeout value</li>
</ul>
<b>ns3::TcpNewReno</b><br>
<ul>
<li><b>ReTxThreshold</b>: Threshold for fast retransmit</li>
<li><b>LimitedTransmit</b>: Enable limited transmit</li>
</ul>
<b>ns3::TcpSocketBase</b><br>
<ul>
<li><b>MaxSegLifetime</b>: Maximum segment lifetime in seconds, use for TIME_WAIT state transition to CLOSED state</li>
<li><b>MaxWindowSize</b>: Max size of advertised window</li>
<li><b>IcmpCallback</b>: Callback invoked whenever an icmp error is received on this socket.</li>
<li><b>IcmpCallback6</b>: Callback invoked whenever an icmpv6 error is received on this socket.</li>
<li><b>WindowScaling</b>: Enable or disable Window Scaling option</li>
<li><b>Timestamp</b>: Enable or disable Timestamp option</li>
</ul>
<b>ns3::TcpSocket</b><br>
<ul>
<li><b>SndBufSize</b>: TcpSocket maximum transmit buffer size (bytes)</li>
<li><b>RcvBufSize</b>: TcpSocket maximum receive buffer size (bytes)</li>
<li><b>SegmentSize</b>: TCP maximum segment size in bytes (may be adjusted based on MTU discovery)</li>
<li><b>InitialSlowStartThreshold</b>: TCP initial slow start threshold (bytes)</li>
<li><b>InitialCwnd</b>: TCP initial congestion window size (segments)</li>
<li><b>ConnTimeout</b>: TCP retransmission timeout when opening connection (seconds)</li>
<li><b>ConnCount</b>: Number of connection attempts (SYN retransmissions) before returning failure</li>
<li><b>DelAckTimeout</b>: Timeout value for TCP delayed acks, in seconds</li>
<li><b>DelAckCount</b>: Number of packets to wait before sending a TCP ack</li>
<li><b>TcpNoDelay</b>: Set to true to disable Nagle's algorithm</li>
<li><b>PersistTimeout</b>: Persist timeout to probe for rx window</li>
</ul>
<b>ns3::ArpCache</b><br>
<ul>
<li><b>AliveTimeout</b>: When this timeout expires, the matching cache entry needs refreshing</li>
<li><b>DeadTimeout</b>: When this timeout expires, a new attempt to resolve the matching entry is made</li>
<li><b>WaitReplyTimeout</b>: When this timeout expires, the cache entries will be scanned and entries in WaitReply state will resend ArpRequest unless MaxRetries has been exceeded, in which case the entry is marked dead</li>
<li><b>MaxRetries</b>: Number of retransmissions of ArpRequest before marking dead</li>
<li><b>PendingQueueSize</b>: The size of the queue for packets pending an arp reply.</li>
</ul>
<b>ns3::ArpL3Protocol</b><br>
<ul>
<li><b>CacheList</b>: The list of ARP caches</li>
<li><b>RequestJitter</b>: The jitter in ms a node is allowed to wait before sending an ARP request. Some jitter aims to prevent collisions. By default, the model will wait for a duration in ms defined by a uniform random-variable between 0 and RequestJitter</li>
</ul>
<b>ns3::UdpSocketImpl</b><br>
<ul>
<li><b>IcmpCallback</b>: Callback invoked whenever an icmp error is received on this socket.</li>
<li><b>IcmpCallback6</b>: Callback invoked whenever an icmpv6 error is received on this socket.</li>
</ul>
<b>ns3::UdpSocket</b><br>
<ul>
<li><b>RcvBufSize</b>: UdpSocket maximum receive buffer size (bytes)</li>
<li><b>IpTtl</b>: socket-specific TTL for unicast IP packets (if non-zero)</li>
<li><b>IpMulticastTtl</b>: socket-specific TTL for multicast IP packets (if non-zero)</li>
<li><b>IpMulticastIf</b>: interface index for outgoing multicast on this socket; -1 indicates to use default interface</li>
<li><b>IpMulticastLoop</b>: whether outgoing multicast sent also to loopback interface</li>
<li><b>MtuDiscover</b>: If enabled, every outgoing ip packet will have the DF flag set.</li>
</ul>
<b>ns3::Ipv4RawSocketImpl</b><br>
<ul>
<li><b>Protocol</b>: Protocol number to match.</li>
<li><b>IcmpFilter</b>: Any icmp header whose type field matches a bit in this filter is dropped. Type must be less than 32.</li>
<li><b>IpHeaderInclude</b>: Include IP Header information (a.k.a setsockopt (IP_HDRINCL)).</li>
</ul>
<b>ns3::NdiscCache</b><br>
<ul>
<li><b>UnresolvedQueueSize</b>: Size of the queue for packets pending an NA reply.</li>
</ul>
<b>ns3::Ipv6L3Protocol</b><br>
<ul>
<li><b>DefaultTtl</b>: The TTL value set by default on all outgoing packets generated on this node.</li>
<li><b>DefaultTclass</b>: The TCLASS value set by default on all outgoing packets generated on this node.</li>
<li><b>InterfaceList</b>: The set of IPv6 interfaces associated to this IPv6 stack.</li>
<li><b>SendIcmpv6Redirect</b>: Send the ICMPv6 Redirect when appropriate.</li>
</ul>
<b>ns3::Ipv6</b><br>
<ul>
<li><b>IpForward</b>: Globally enable or disable IP forwarding for all current and future IPv6 devices.</li>
<li><b>MtuDiscover</b>: If disabled, every interface will have its MTU set to 1280 bytes.</li>
</ul>
<b>ns3::Ipv6RawSocketImpl</b><br>
<ul>
<li><b>Protocol</b>: Protocol number to match.</li>
</ul>
<b>ns3::Ipv6Extension</b><br>
<ul>
<li><b>ExtensionNumber</b>: The IPv6 extension number.</li>
</ul>
<b>ns3::Ipv6ExtensionRoutingDemux</b><br>
<ul>
<li><b>Routing Extensions</b>: The set of IPv6 Routing extensions registered with this demux.</li>
</ul>
<b>ns3::Ipv6ExtensionDemux</b><br>
<ul>
<li><b>Extensions</b>: The set of IPv6 extensions registered with this demux.</li>
</ul>
<b>ns3::Ipv6Option</b><br>
<ul>
<li><b>OptionNumber</b>: The IPv6 option number.</li>
</ul>
<b>ns3::Ipv6OptionDemux</b><br>
<ul>
<li><b>Options</b>: The set of IPv6 options registered with this demux.</li>
</ul>
<b>ns3::Icmpv6L4Protocol</b><br>
<ul>
<li><b>DAD</b>: Always do DAD check.</li>
<li><b>SolicitationJitter</b>: The jitter in ms a node is allowed to wait before sending any solicitation . Some jitter aims to prevent collisions. By default, the model will wait for a duration in ms defined by a uniform random-variable between 0 and SolicitationJitter</li>
</ul>
<b>ns3::TcpTahoe</b><br>
<ul>
<li><b>ReTxThreshold</b>: Threshold for fast retransmit</li>
</ul>
<b>ns3::TcpReno</b><br>
<ul>
<li><b>ReTxThreshold</b>: Threshold for fast retransmit</li>
</ul>
<b>ns3::TcpWestwood</b><br>
<ul>
<li><b>FilterType</b>: Use this to choose no filter or Tustin's approximation filter</li>
<li><b>ProtocolType</b>: Use this to let the code run as Westwood or WestwoodPlus</li>
</ul>
<b>ns3::CoDelQueue</b><br>
<ul>
<li><b>Mode</b>: Whether to use Bytes (see MaxBytes) or Packets (see MaxPackets) as the maximum queue size metric.</li>
<li><b>MaxPackets</b>: The maximum number of packets accepted by this CoDelQueue.</li>
<li><b>MaxBytes</b>: The maximum number of bytes accepted by this CoDelQueue.</li>
<li><b>MinBytes</b>: The CoDel algorithm minbytes parameter.</li>
<li><b>Interval</b>: The CoDel algorithm interval</li>
<li><b>Target</b>: The CoDel algorithm target queue delay</li>
</ul>
<b>ns3::Ipv4GlobalRouting</b><br>
<ul>
<li><b>RandomEcmpRouting</b>: Set to true if packets are randomly routed among ECMP; set to false for using only one route consistently</li>
<li><b>RespondToInterfaceEvents</b>: Set to true if you want to dynamically recompute the global routes upon Interface notification events (up/down, or add/remove address)</li>
</ul>
<b>ns3::Ipv6PmtuCache</b><br>
<ul>
<li><b>CacheExpiryTime</b>: Validity time for a Path MTU entry. Default is 10 minutes, minimum is 5 minutes.</li>
</ul>
<b>ns3::RipNg</b><br>
<ul>
<li><b>UnsolicitedRoutingUpdate</b>: The time between two Unsolicited Routing Updates.</li>
<li><b>StartupDelay</b>: Maximum random delay for protocol startup (send route requests).</li>
<li><b>TimeoutDelay</b>: The delay to invalidate a route.</li>
<li><b>GarbageCollectionDelay</b>: The delay to delete an expired route.</li>
<li><b>MinTriggeredCooldown</b>: Min cooldown delay after a Triggered Update.</li>
<li><b>MaxTriggeredCooldown</b>: Max cooldown delay after a Triggered Update.</li>
<li><b>SplitHorizon</b>: Split Horizon strategy.</li>
</ul>
<b>ns3::WimaxNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
<li><b>Channel</b>: The channel attached to this device.</li>
<li><b>RTG</b>: receive/transmit transition gap.</li>
<li><b>TTG</b>: transmit/receive transition gap.</li>
<li><b>ConnectionManager</b>: The connection manager attached to this device.</li>
<li><b>BurstProfileManager</b>: The burst profile manager attached to this device.</li>
<li><b>BandwidthManager</b>: The bandwidth manager attached to this device.</li>
<li><b>InitialRangingConnection</b>: Initial ranging connection</li>
<li><b>BroadcastConnection</b>: Broadcast connection</li>
</ul>
<b>ns3::BaseStationNetDevice</b><br>
<ul>
<li><b>BSScheduler</b>: Downlink Scheduler for BS</li>
<li><b>InitialRangInterval</b>: Time between Initial Ranging regions assigned by the BS. Maximum is 2s</li>
<li><b>DcdInterval</b>: Time between transmission of DCD messages. Maximum value is 10s.</li>
<li><b>UcdInterval</b>: Time between transmission of UCD messages. Maximum value is 10s.</li>
<li><b>IntervalT8</b>: Wait for DSA/DSC Acknowledge timeout. Maximum 300ms.</li>
<li><b>RangReqOppSize</b>: The ranging opportunity size in symbols</li>
<li><b>BwReqOppSize</b>: The bandwidth request opportunity size in symbols</li>
<li><b>MaxRangCorrectionRetries</b>: Number of retries on contention Ranging Requests</li>
<li><b>SSManager</b>: The ss manager attached to this device.</li>
<li><b>Scheduler</b>: The BS scheduler attached to this device.</li>
<li><b>LinkManager</b>: The link manager attached to this device.</li>
<li><b>UplinkScheduler</b>: The uplink scheduler attached to this device.</li>
<li><b>BsIpcsPacketClassifier</b>: The uplink IP packet classifier attached to this device.</li>
<li><b>ServiceFlowManager</b>: The service flow manager attached to this device.</li>
</ul>
<b>ns3::SubscriberStationNetDevice</b><br>
<ul>
<li><b>BasicConnection</b>: Basic connection</li>
<li><b>PrimaryConnection</b>: Primary connection</li>
<li><b>LostDlMapInterval</b>: Time since last received DL-MAP message before downlink synchronization is considered lost. Maximum is 600ms</li>
<li><b>LostUlMapInterval</b>: Time since last received UL-MAP before uplink synchronization is considered lost, maximum is 600.</li>
<li><b>MaxDcdInterval</b>: Maximum time between transmission of DCD messages. Maximum is 10s</li>
<li><b>MaxUcdInterval</b>: Maximum time between transmission of UCD messages. Maximum is 10s</li>
<li><b>IntervalT1</b>: Wait for DCD timeout. Maximum is 5*maxDcdInterval</li>
<li><b>IntervalT2</b>: Wait for broadcast ranging timeout, i.e., wait for initial ranging opportunity. Maximum is 5*Ranging interval</li>
<li><b>IntervalT3</b>: ranging Response reception timeout following the transmission of a ranging request. Maximum is 200ms</li>
<li><b>IntervalT7</b>: wait for DSA/DSC/DSD Response timeout. Maximum is 1s</li>
<li><b>IntervalT12</b>: Wait for UCD descriptor.Maximum is 5*MaxUcdInterval</li>
<li><b>IntervalT20</b>: Time the SS searches for preambles on a given channel. Minimum is 2 MAC frames</li>
<li><b>IntervalT21</b>: time the SS searches for (decodable) DL-MAP on a given channel</li>
<li><b>MaxContentionRangingRetries</b>: Number of retries on contention Ranging Requests</li>
<li><b>SSScheduler</b>: The ss scheduler attached to this device.</li>
<li><b>LinkManager</b>: The ss link manager attached to this device.</li>
<li><b>Classifier</b>: The ss classifier attached to this device.</li>
</ul>
<b>ns3::WimaxPhy</b><br>
<ul>
<li><b>Channel</b>: Wimax channel</li>
<li><b>FrameDuration</b>: The frame duration in seconds.</li>
<li><b>Frequency</b>: The central frequency in KHz.</li>
<li><b>Bandwidth</b>: The channel bandwidth in Hz.</li>
</ul>
<b>ns3::WimaxConnection</b><br>
<ul>
<li><b>Type</b>: Connection type</li>
<li><b>TxQueue</b>: Transmit queue</li>
</ul>
<b>ns3::SimpleOfdmWimaxPhy</b><br>
<ul>
<li><b>NoiseFigure</b>: Loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver.</li>
<li><b>TxPower</b>: Transmission power (dB).</li>
<li><b>G</b>: This is the ratio of CP time to useful time.</li>
<li><b>TxGain</b>: Transmission gain (dB).</li>
<li><b>RxGain</b>: Reception gain (dB).</li>
<li><b>Nfft</b>: FFT size</li>
<li><b>TraceFilePath</b>: Path to the directory containing SNR to block error rate files</li>
</ul>
<b>ns3::UplinkSchedulerMBQoS</b><br>
<ul>
<li><b>WindowInterval</b>: The time to wait to reset window</li>
</ul>
<b>ns3::WimaxMacQueue</b><br>
<ul>
<li><b>MaxSize</b>: Maximum size</li>
</ul>
<b>ns3::ConfigStore</b><br>
<ul>
<li><b>Mode</b>: Configuration mode</li>
<li><b>Filename</b>: The file where the configuration should be saved to or loaded from.</li>
<li><b>FileFormat</b>: Type of file format</li>
</ul>
<b>ns3::Building</b><br>
<ul>
<li><b>NRoomsX</b>: The number of rooms in the X axis.</li>
<li><b>NRoomsY</b>: The number of rooms in the Y axis.</li>
<li><b>NFloors</b>: The number of floors of this building.</li>
<li><b>Id</b>: The id (unique integer) of this Building.</li>
<li><b>Boundaries</b>: The boundaries of this Building as a value of type ns3::Box</li>
<li><b>Type</b>: The type of building</li>
<li><b>ExternalWallsType</b>: The type of material of which the external walls are made</li>
</ul>
<b>ns3::BuildingListPriv</b><br>
<ul>
<li><b>BuildingList</b>: The list of all buildings created during the simulation.</li>
</ul>
<b>ns3::ItuR1238PropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The Frequency  (default is 2.106 GHz).</li>
</ul>
<b>ns3::BuildingsPropagationLossModel</b><br>
<ul>
<li><b>ShadowSigmaOutdoor</b>: Standard deviation of the normal distribution used for calculate the shadowing for outdoor nodes</li>
<li><b>ShadowSigmaIndoor</b>: Standard deviation of the normal distribution used for calculate the shadowing for indoor nodes </li>
<li><b>ShadowSigmaExtWalls</b>: Standard deviation of the normal distribution used for calculate the shadowing due to ext walls </li>
<li><b>InternalWallLoss</b>: Additional loss for each internal wall [dB]</li>
</ul>
<b>ns3::HybridBuildingsPropagationLossModel</b><br>
<ul>
<li><b>Frequency</b>: The Frequency  (default is 2.106 GHz).</li>
<li><b>Los2NlosThr</b>:  Threshold from LoS to NLoS in ITU 1411 [m].</li>
<li><b>Environment</b>: Environment Scenario</li>
<li><b>CitySize</b>: Dimension of the city</li>
<li><b>RooftopLevel</b>: The height of the rooftop level in meters</li>
</ul>
<b>ns3::RandomBuildingPositionAllocator</b><br>
<ul>
<li><b>WithReplacement</b>: If true, the building will be randomly selected with replacement. If false, no replacement will occur, until the list of buildings to select becomes empty, at which point it will be filled again with the list of all buildings.</li>
</ul>
<b>ns3::GridBuildingAllocator</b><br>
<ul>
<li><b>GridWidth</b>: The number of objects layed out on a line.</li>
<li><b>MinX</b>: The x coordinate where the grid starts.</li>
<li><b>MinY</b>: The y coordinate where the grid starts.</li>
<li><b>LengthX</b>:  the length of the wall of each building along the X axis.</li>
<li><b>LengthY</b>:  the length of the wall of each building along the X axis.</li>
<li><b>DeltaX</b>: The x space between buildings.</li>
<li><b>DeltaY</b>: The y space between buildings.</li>
<li><b>Height</b>: The height of the building (roof level)</li>
<li><b>LayoutType</b>: The type of layout.</li>
</ul>
<b>ns3::BulkSendApplication</b><br>
<ul>
<li><b>SendSize</b>: The amount of data to send each time.</li>
<li><b>Remote</b>: The address of the destination</li>
<li><b>MaxBytes</b>: The total number of bytes to send. Once these bytes are sent, no data  is sent again. The value zero means that there is no limit.</li>
<li><b>Protocol</b>: The type of protocol to use.</li>
</ul>
<b>ns3::OnOffApplication</b><br>
<ul>
<li><b>DataRate</b>: The data rate in on state.</li>
<li><b>PacketSize</b>: The size of packets sent in on state</li>
<li><b>Remote</b>: The address of the destination</li>
<li><b>OnTime</b>: A RandomVariableStream used to pick the duration of the 'On' state.</li>
<li><b>OffTime</b>: A RandomVariableStream used to pick the duration of the 'Off' state.</li>
<li><b>MaxBytes</b>: The total number of bytes to send. Once these bytes are sent, no packet is sent again, even in on state. The value zero means that there is no limit.</li>
<li><b>Protocol</b>: The type of protocol to use.</li>
</ul>
<b>ns3::PacketSink</b><br>
<ul>
<li><b>Local</b>: The Address on which to Bind the rx socket.</li>
<li><b>Protocol</b>: The type id of the protocol to use for the rx socket.</li>
</ul>
<b>ns3::Ping6</b><br>
<ul>
<li><b>MaxPackets</b>: The maximum number of packets the application will send</li>
<li><b>Interval</b>: The time to wait between packets</li>
<li><b>RemoteIpv6</b>: The Ipv6Address of the outbound packets</li>
<li><b>LocalIpv6</b>: Local Ipv6Address of the sender</li>
<li><b>PacketSize</b>: Size of packets generated</li>
</ul>
<b>ns3::Radvd</b><br>
<ul>
<li><b>AdvertisementJitter</b>: Uniform variable to provide jitter between min and max values of AdvInterval</li>
</ul>
<b>ns3::UdpClient</b><br>
<ul>
<li><b>MaxPackets</b>: The maximum number of packets the application will send</li>
<li><b>Interval</b>: The time to wait between packets</li>
<li><b>RemoteAddress</b>: The destination Address of the outbound packets</li>
<li><b>RemotePort</b>: The destination port of the outbound packets</li>
<li><b>PacketSize</b>: Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.</li>
</ul>
<b>ns3::UdpServer</b><br>
<ul>
<li><b>Port</b>: Port on which we listen for incoming packets.</li>
<li><b>PacketWindowSize</b>: The size of the window used to compute the packet loss. This value should be a multiple of 8.</li>
</ul>
<b>ns3::UdpTraceClient</b><br>
<ul>
<li><b>RemoteAddress</b>: The destination Address of the outbound packets</li>
<li><b>RemotePort</b>: The destination port of the outbound packets</li>
<li><b>MaxPacketSize</b>: The maximum size of a packet (including the SeqTsHeader, 12 bytes).</li>
<li><b>TraceFilename</b>: Name of file to load a trace from. By default, uses a hardcoded trace.</li>
</ul>
<b>ns3::UdpEchoClient</b><br>
<ul>
<li><b>MaxPackets</b>: The maximum number of packets the application will send</li>
<li><b>Interval</b>: The time to wait between packets</li>
<li><b>RemoteAddress</b>: The destination Address of the outbound packets</li>
<li><b>RemotePort</b>: The destination port of the outbound packets</li>
<li><b>PacketSize</b>: Size of echo data in outbound packets</li>
</ul>
<b>ns3::UdpEchoServer</b><br>
<ul>
<li><b>Port</b>: Port on which we listen for incoming packets.</li>
</ul>
<b>ns3::V4Ping</b><br>
<ul>
<li><b>Remote</b>: The address of the machine we want to ping.</li>
<li><b>Verbose</b>: Produce usual output.</li>
<li><b>Interval</b>: Wait  interval  seconds between sending each packet.</li>
<li><b>Size</b>: The number of data bytes to be sent, real packet will be 8 (ICMP) + 20 (IP) bytes longer.</li>
</ul>
<b>ns3::VirtualNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
</ul>
<b>ns3::CsmaNetDevice</b><br>
<ul>
<li><b>Address</b>: The MAC address of this device.</li>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>EncapsulationMode</b>: The link-layer encapsulation type to use.</li>
<li><b>SendEnable</b>: Enable or disable the transmitter section of the device.</li>
<li><b>ReceiveEnable</b>: Enable or disable the receiver section of the device.</li>
<li><b>ReceiveErrorModel</b>: The receiver error model used to simulate packet loss</li>
<li><b>TxQueue</b>: A queue to use as the transmit queue in the device.</li>
</ul>
<b>ns3::CsmaChannel</b><br>
<ul>
<li><b>DataRate</b>: The transmission data rate to be provided to devices connected to the channel</li>
<li><b>Delay</b>: Transmission delay through the channel</li>
</ul>
<b>ns3::LteSpectrumPhy</b><br>
<ul>
<li><b>DataErrorModelEnabled</b>: Activate/Deactivate the error model of data (TBs of PDSCH and PUSCH) [by default is active].</li>
<li><b>CtrlErrorModelEnabled</b>: Activate/Deactivate the error model of control (PCFICH-PDCCH decodification) [by default is active].</li>
</ul>
<b>ns3::LteEnbPhy</b><br>
<ul>
<li><b>TxPower</b>: Transmission power in dBm</li>
<li><b>NoiseFigure</b>: Loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver. According to Wikipedia (http://en.wikipedia.org/wiki/Noise_figure), this is "the difference in decibels (dB) between the noise output of the actual receiver to the noise output of an  ideal receiver with the same overall gain and bandwidth when the receivers  are connected to sources at the standard noise temperature T0." In this model, we consider T0 = 290K.</li>
<li><b>MacToChannelDelay</b>: The delay in TTI units that occurs between a scheduling decision in the MAC and the actual start of the transmission by the PHY. This is intended to be used to model the latency of real PHY and MAC implementations.</li>
<li><b>UeSinrSamplePeriod</b>: The sampling period for reporting UEs' SINR stats (default value 1)</li>
<li><b>InterferenceSamplePeriod</b>: The sampling period for reporting interference stats (default value 1)</li>
<li><b>DlSpectrumPhy</b>: The downlink LteSpectrumPhy associated to this LtePhy</li>
<li><b>UlSpectrumPhy</b>: The uplink LteSpectrumPhy associated to this LtePhy</li>
</ul>
<b>ns3::LteUePhy</b><br>
<ul>
<li><b>TxPower</b>: Transmission power in dBm</li>
<li><b>NoiseFigure</b>: Loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver. According to Wikipedia (http://en.wikipedia.org/wiki/Noise_figure), this is "the difference in decibels (dB) between the noise output of the actual receiver to the noise output of an  ideal receiver with the same overall gain and bandwidth when the receivers  are connected to sources at the standard noise temperature T0." In this model, we consider T0 = 290K.</li>
<li><b>TxMode1Gain</b>: Transmission mode 1 gain in dB</li>
<li><b>TxMode2Gain</b>: Transmission mode 2 gain in dB</li>
<li><b>TxMode3Gain</b>: Transmission mode 3 gain in dB</li>
<li><b>TxMode4Gain</b>: Transmission mode 4 gain in dB</li>
<li><b>TxMode5Gain</b>: Transmission mode 5 gain in dB</li>
<li><b>TxMode6Gain</b>: Transmission mode 6 gain in dB</li>
<li><b>TxMode7Gain</b>: Transmission mode 7 gain in dB</li>
<li><b>RsrpSinrSamplePeriod</b>: The sampling period for reporting RSRP-SINR stats (default value 1)</li>
<li><b>DlSpectrumPhy</b>: The downlink LteSpectrumPhy associated to this LtePhy</li>
<li><b>UlSpectrumPhy</b>: The uplink LteSpectrumPhy associated to this LtePhy</li>
<li><b>RsrqUeMeasThreshold</b>: Receive threshold for PSS on RSRQ [dB]</li>
<li><b>UeMeasurementsFilterPeriod</b>: Time period for reporting UE measurements (default 200 ms.) </li>
<li><b>EnableUplinkPowerControl</b>: If true Uplink Power Control will be enabled</li>
</ul>
<b>ns3::LteAmc</b><br>
<ul>
<li><b>Ber</b>: The requested BER in assigning MCS (default is 0.00005).</li>
<li><b>AmcModel</b>: AMC model used to assign CQI</li>
</ul>
<b>ns3::UeManager</b><br>
<ul>
<li><b>DataRadioBearerMap</b>: List of UE DataRadioBearerInfo by DRBID.</li>
<li><b>Srb0</b>: SignalingRadioBearerInfo for SRB0</li>
<li><b>Srb1</b>: SignalingRadioBearerInfo for SRB1</li>
<li><b>C-RNTI</b>: Cell Radio Network Temporary Identifier</li>
</ul>
<b>ns3::LteEnbRrc</b><br>
<ul>
<li><b>UeMap</b>: List of UeManager by C-RNTI.</li>
<li><b>DefaultTransmissionMode</b>: The default UEs' transmission mode (0: SISO)</li>
<li><b>EpsBearerToRlcMapping</b>: Specify which type of RLC will be used for each type of EPS bearer. </li>
<li><b>SystemInformationPeriodicity</b>: The interval for sending system information (Time value)</li>
<li><b>SrsPeriodicity</b>: The SRS periodicity in milliseconds</li>
<li><b>ConnectionRequestTimeoutDuration</b>: After a RA attempt, if no RRC CONNECTION REQUEST is received before this time, the UE context is destroyed. Must account for reception of RAR and transmission of RRC CONNECTION REQUEST over UL GRANT.</li>
<li><b>ConnectionSetupTimeoutDuration</b>: After accepting connection request, if no RRC CONNECTION SETUP COMPLETE is received before this time, the UE context is destroyed. Must account for the UE's reception of RRC CONNECTION SETUP and transmission of RRC CONNECTION SETUP COMPLETE.</li>
<li><b>ConnectionRejectedTimeoutDuration</b>: Time to wait between sending a RRC CONNECTION REJECT and destroying the UE context</li>
<li><b>HandoverJoiningTimeoutDuration</b>: After accepting a handover request, if no RRC CONNECTION RECONFIGURATION COMPLETE is received before this time, the UE context is destroyed. Must account for reception of X2 HO REQ ACK by source eNB, transmission of the Handover Command, non-contention-based random access and reception of the RRC CONNECTION RECONFIGURATION COMPLETE message.</li>
<li><b>HandoverLeavingTimeoutDuration</b>: After issuing a Handover Command, if neither RRC CONNECTION RE-ESTABLISHMENT nor X2 UE Context Release has been previously received, the UE context is destroyed.</li>
<li><b>QRxLevMin</b>: One of information transmitted within the SIB1 message, indicating the required minimum RSRP level that any UE must receive from this cell before it is allowed to camp to this cell. The default value -70 corresponds to -140 dBm and is the lowest possible value as defined by Section 6.3.4 of 3GPP TS 36.133. This restriction, however, only applies to initial cell selection and EPC-enabled simulation.</li>
<li><b>AdmitHandoverRequest</b>: Whether to admit an X2 handover request from another eNB</li>
<li><b>AdmitRrcConnectionRequest</b>: Whether to admit a connection request from a UE</li>
<li><b>RsrpFilterCoefficient</b>: Determines the strength of smoothing effect induced by layer 3 filtering of RSRP in all attached UE; if set to 0, no layer 3 filtering is applicable</li>
<li><b>RsrqFilterCoefficient</b>: Determines the strength of smoothing effect induced by layer 3 filtering of RSRQ in all attached UE; if set to 0, no layer 3 filtering is applicable</li>
</ul>
<b>ns3::LteUeRrc</b><br>
<ul>
<li><b>DataRadioBearerMap</b>: List of UE RadioBearerInfo for Data Radio Bearers by LCID.</li>
<li><b>Srb0</b>: SignalingRadioBearerInfo for SRB0</li>
<li><b>Srb1</b>: SignalingRadioBearerInfo for SRB1</li>
<li><b>CellId</b>: Serving cell identifier</li>
<li><b>C-RNTI</b>: Cell Radio Network Temporary Identifier</li>
<li><b>T300</b>: Timer for the RRC Connection Establishment procedure (i.e., the procedure is deemed as failed if it takes longer than this)</li>
</ul>
<b>ns3::LteRlcTm</b><br>
<ul>
<li><b>MaxTxBufferSize</b>: Maximum Size of the Transmission Buffer (in Bytes)</li>
</ul>
<b>ns3::LteRlcUm</b><br>
<ul>
<li><b>MaxTxBufferSize</b>: Maximum Size of the Transmission Buffer (in Bytes)</li>
</ul>
<b>ns3::LteRlcAm</b><br>
<ul>
<li><b>PollRetransmitTimer</b>: Value of the t-PollRetransmit (See section 7.3 of 3GPP TS 36.322)</li>
<li><b>TxOpportunityForRetxAlwaysBigEnough</b>: If true, always pretend that the size of a TxOpportunity is big enough for retransmission. If false (default and realistic behavior), no retx is performed unless the corresponding TxOpportunity is big enough.</li>
</ul>
<b>ns3::LteNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
</ul>
<b>ns3::LteEnbNetDevice</b><br>
<ul>
<li><b>LteEnbRrc</b>: The RRC associated to this EnbNetDevice</li>
<li><b>LteHandoverAlgorithm</b>: The handover algorithm associated to this EnbNetDevice</li>
<li><b>LteAnr</b>: The automatic neighbour relation function associated to this EnbNetDevice</li>
<li><b>LteFfrAlgorithm</b>: The FFR algorithm associated to this EnbNetDevice</li>
<li><b>LteEnbMac</b>: The MAC associated to this EnbNetDevice</li>
<li><b>FfMacScheduler</b>: The scheduler associated to this EnbNetDevice</li>
<li><b>LteEnbPhy</b>: The PHY associated to this EnbNetDevice</li>
<li><b>UlBandwidth</b>: Uplink Transmission Bandwidth Configuration in number of Resource Blocks</li>
<li><b>DlBandwidth</b>: Downlink Transmission Bandwidth Configuration in number of Resource Blocks</li>
<li><b>CellId</b>: Cell Identifier</li>
<li><b>DlEarfcn</b>: Downlink E-UTRA Absolute Radio Frequency Channel Number (EARFCN) as per 3GPP 36.101 Section 5.7.3. </li>
<li><b>UlEarfcn</b>: Uplink E-UTRA Absolute Radio Frequency Channel Number (EARFCN) as per 3GPP 36.101 Section 5.7.3. </li>
<li><b>CsgId</b>: The Closed Subscriber Group (CSG) identity that this eNodeB belongs to</li>
<li><b>CsgIndication</b>: If true, only UEs which are members of the CSG (i.e. same CSG ID) can gain access to the eNodeB, therefore enforcing closed access mode. Otherwise, the eNodeB operates as a non-CSG cell and implements open access mode.</li>
</ul>
<b>ns3::LteUeNetDevice</b><br>
<ul>
<li><b>EpcUeNas</b>: The NAS associated to this UeNetDevice</li>
<li><b>LteUeRrc</b>: The RRC associated to this UeNetDevice</li>
<li><b>LteUeMac</b>: The MAC associated to this UeNetDevice</li>
<li><b>LteUePhy</b>: The PHY associated to this UeNetDevice</li>
<li><b>Imsi</b>: International Mobile Subscriber Identity assigned to this UE</li>
<li><b>DlEarfcn</b>: Downlink E-UTRA Absolute Radio Frequency Channel Number (EARFCN) as per 3GPP 36.101 Section 5.7.3. </li>
<li><b>CsgId</b>: The Closed Subscriber Group (CSG) identity that this UE is associated with, i.e., giving the UE access to cells which belong to this particular CSG. This restriction only applies to initial cell selection and EPC-enabled simulation. This does not revoke the UE's access to non-CSG cells. </li>
</ul>
<b>ns3::LteHelper</b><br>
<ul>
<li><b>Scheduler</b>: The type of scheduler to be used for eNBs. The allowed values for this attributes are the type names of any class inheriting from ns3::FfMacScheduler.</li>
<li><b>FfrAlgorithm</b>: The type of FFR algorithm to be used for eNBs. The allowed values for this attributes are the type names of any class inheriting from ns3::LteFfrAlgorithm.</li>
<li><b>HandoverAlgorithm</b>: The type of handover algorithm to be used for eNBs. The allowed values for this attributes are the type names of any class inheriting from ns3::LteHandoverAlgorithm.</li>
<li><b>PathlossModel</b>: The type of pathloss model to be used. The allowed values for this attributes are the type names of any class inheriting from ns3::PropagationLossModel.</li>
<li><b>FadingModel</b>: The type of fading model to be used.The allowed values for this attributes are the type names of any class inheriting from ns3::SpectrumPropagationLossModel.If the type is set to an empty string, no fading model is used.</li>
<li><b>UseIdealRrc</b>: If true, LteRrcProtocolIdeal will be used for RRC signaling. If false, LteRrcProtocolReal will be used.</li>
<li><b>AnrEnabled</b>: Activate or deactivate Automatic Neighbour Relation function</li>
<li><b>UsePdschForCqiGeneration</b>: If true, DL-CQI will be calculated from PDCCH as signal and PDSCH as interference If false, DL-CQI will be calculated from PDCCH as signal and PDCCH as interference  </li>
</ul>
<b>ns3::PointToPointEpcHelper</b><br>
<ul>
<li><b>S1uLinkDataRate</b>: The data rate to be used for the next S1-U link to be created</li>
<li><b>S1uLinkDelay</b>: The delay to be used for the next S1-U link to be created</li>
<li><b>S1uLinkMtu</b>: The MTU of the next S1-U link to be created. Note that, because of the additional GTP/UDP/IP tunneling overhead, you need a MTU larger than the end-to-end MTU that you want to support.</li>
<li><b>X2LinkDataRate</b>: The data rate to be used for the next X2 link to be created</li>
<li><b>X2LinkDelay</b>: The delay to be used for the next X2 link to be created</li>
<li><b>X2LinkMtu</b>: The MTU of the next X2 link to be created. Note that, because of some big X2 messages, you need a big MTU.</li>
</ul>
<b>ns3::RadioBearerStatsCalculator</b><br>
<ul>
<li><b>StartTime</b>: Start time of the on going epoch.</li>
<li><b>EpochDuration</b>: Epoch duration.</li>
<li><b>DlRlcOutputFilename</b>: Name of the file where the downlink results will be saved.</li>
<li><b>UlRlcOutputFilename</b>: Name of the file where the uplink results will be saved.</li>
<li><b>DlPdcpOutputFilename</b>: Name of the file where the downlink results will be saved.</li>
<li><b>UlPdcpOutputFilename</b>: Name of the file where the uplink results will be saved.</li>
</ul>
<b>ns3::PhyStatsCalculator</b><br>
<ul>
<li><b>DlRsrpSinrFilename</b>: Name of the file where the RSRP/SINR statistics will be saved.</li>
<li><b>UlSinrFilename</b>: Name of the file where the UE SINR statistics will be saved.</li>
<li><b>UlInterferenceFilename</b>: Name of the file where the interference statistics will be saved.</li>
</ul>
<b>ns3::MacStatsCalculator</b><br>
<ul>
<li><b>DlOutputFilename</b>: Name of the file where the downlink results will be saved.</li>
<li><b>UlOutputFilename</b>: Name of the file where the uplink results will be saved.</li>
</ul>
<b>ns3::PhyTxStatsCalculator</b><br>
<ul>
<li><b>DlTxOutputFilename</b>: Name of the file where the downlink results will be saved.</li>
<li><b>UlTxOutputFilename</b>: Name of the file where the uplink results will be saved.</li>
</ul>
<b>ns3::PhyRxStatsCalculator</b><br>
<ul>
<li><b>DlRxOutputFilename</b>: Name of the file where the downlink results will be saved.</li>
<li><b>UlRxOutputFilename</b>: Name of the file where the uplink results will be saved.</li>
</ul>
<b>ns3::RadioEnvironmentMapHelper</b><br>
<ul>
<li><b>ChannelPath</b>: The path to the channel for which the Radio Environment Map is to be generated</li>
<li><b>OutputFile</b>: the filename to which the Radio Environment Map is saved</li>
<li><b>XMin</b>: The min x coordinate of the map.</li>
<li><b>YMin</b>: The min y coordinate of the map.</li>
<li><b>XMax</b>: The max x coordinate of the map.</li>
<li><b>YMax</b>: The max y coordinate of the map.</li>
<li><b>XRes</b>: The resolution (number of points) of the map along the x axis.</li>
<li><b>YRes</b>: The resolution (number of points) of the map along the y axis.</li>
<li><b>Z</b>: The value of the z coordinate for which the map is to be generated</li>
<li><b>StopWhenDone</b>: If true, Simulator::Stop () will be called as soon as the REM has been generated</li>
<li><b>NoisePower</b>: the power of the measuring instrument noise, in Watts. Default to a kT of -174 dBm with a noise figure of 9 dB and a bandwidth of 25 LTE Resource Blocks</li>
<li><b>MaxPointsPerIteration</b>: Maximum number of REM points to be calculated per iteration. Every point consumes approximately 5KB of memory.</li>
<li><b>Earfcn</b>: E-UTRA Absolute Radio Frequency Channel Number (EARFCN) as per 3GPP 36.101 Section 5.7.3. </li>
<li><b>Bandwidth</b>: Transmission Bandwidth Configuration (in number of RBs) over which the SINR will be calculated</li>
<li><b>UseDataChannel</b>: If true, REM will be generated for PDSCH and for PDCCH otherwise </li>
<li><b>RbId</b>: Resource block Id, for which REM will be generated,default value is -1, what means REM will be averaged from all RBs</li>
</ul>
<b>ns3::LteHexGridEnbTopologyHelper</b><br>
<ul>
<li><b>InterSiteDistance</b>: The distance [m] between nearby sites</li>
<li><b>SectorOffset</b>: The offset [m] in the position for the node of each sector with respect to the center of the three-sector site</li>
<li><b>SiteHeight</b>: The height [m] of each site</li>
<li><b>MinX</b>: The x coordinate where the hex grid starts.</li>
<li><b>MinY</b>: The y coordinate where the hex grid starts.</li>
<li><b>GridWidth</b>: The number of sites in even rows (odd rows will have one additional site).</li>
</ul>
<b>ns3::FfMacScheduler</b><br>
<ul>
<li><b>UlCqiFilter</b>: The filter to apply on UL CQIs received</li>
</ul>
<b>ns3::RrFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::LteEnbMac</b><br>
<ul>
<li><b>NumberOfRaPreambles</b>: how many random access preambles are available for the contention based RACH process</li>
<li><b>PreambleTransMax</b>: Maximum number of random access preamble transmissions</li>
<li><b>RaResponseWindowSize</b>: length of the window (in TTIs) for the reception of the random access response (RAR); the resulting RAR timeout is this value + 3 ms</li>
</ul>
<b>ns3::LteRadioBearerTag</b><br>
<ul>
<li><b>rnti</b>: The rnti that indicates the UE to which packet belongs</li>
<li><b>lcid</b>: The id whithin the UE identifying the logical channel to which the packet belongs</li>
</ul>
<b>ns3::EpsBearerTag</b><br>
<ul>
<li><b>rnti</b>: The rnti that indicates the UE which packet belongs</li>
<li><b>bid</b>: The EPS bearer id within the UE to which the packet belongs</li>
</ul>
<b>ns3::PfFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::FdMtFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::TdMtFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::TtaFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::FdBetFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::TdBetFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::FdTbfqFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>DebtLimit</b>: Flow debt limit (default -625000 bytes)</li>
<li><b>CreditLimit</b>: Flow credit limit (default 625000 bytes)</li>
<li><b>TokenPoolSize</b>: The maximum value of flow token pool (default 1 bytes)</li>
<li><b>CreditableThreshold</b>: Threshold of flow credit (default 0 bytes)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::TdTbfqFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>DebtLimit</b>: Flow debt limit (default -625000 bytes)</li>
<li><b>CreditLimit</b>: Flow credit limit (default 625000 bytes)</li>
<li><b>TokenPoolSize</b>: The maximum value of flow token pool (default 1 bytes)</li>
<li><b>CreditableThreshold</b>: Threshold of flow credit (default 0 bytes)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::PssFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>PssFdSchedulerType</b>: FD scheduler in PSS (default value is PFsch)</li>
<li><b>nMux</b>: The number of UE selected by TD scheduler (default value is 0)</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::CqaFfMacScheduler</b><br>
<ul>
<li><b>CqiTimerThreshold</b>: The number of TTIs a CQI is valid (default 1000 - 1 sec.)</li>
<li><b>CqaMetric</b>: CqaFfMacScheduler metric type that can be: CqaFf, CqaPf</li>
<li><b>HarqEnabled</b>: Activate/Deactivate the HARQ [by default is active].</li>
<li><b>UlGrantMcs</b>: The MCS of the UL grant, must be [0..15] (default 0)</li>
</ul>
<b>ns3::TraceFadingLossModel</b><br>
<ul>
<li><b>TraceFilename</b>: Name of file to load a trace from.</li>
<li><b>TraceLength</b>: The total length of the fading trace (default value 10 s.)</li>
<li><b>SamplesNum</b>: The number of samples the trace is made of (default 10000)</li>
<li><b>WindowSize</b>: The size of the window for the fading trace (default value 0.5 s.)</li>
<li><b>RbNum</b>: The number of RB the trace is made of (default 100)</li>
<li><b>RngStreamSetSize</b>: The number of RNG streams reserved for the fading model. The maximum number of streams that are needed for an LTE FDD scenario is 2 * numUEs * numeNBs.</li>
</ul>
<b>ns3::A2A4RsrqHandoverAlgorithm</b><br>
<ul>
<li><b>ServingCellThreshold</b>: If the RSRQ of the serving cell is worse than this threshold, neighbour cells are consider for handover</li>
<li><b>NeighbourCellOffset</b>: Minimum offset between serving and best neighbour cell to trigger the Handover</li>
</ul>
<b>ns3::A3RsrpHandoverAlgorithm</b><br>
<ul>
<li><b>Hysteresis</b>: Handover margin (hysteresis) in dB (rounded to the nearest multiple of 0.5 dB)</li>
<li><b>TimeToTrigger</b>: Time during which neighbour cell's RSRP must continuously higher than serving cell's RSRP in order to trigger a handover</li>
</ul>
<b>ns3::LteAnr</b><br>
<ul>
<li><b>Threshold</b>: Minimum RSRQ range value required for detecting a neighbour cell</li>
</ul>
<b>ns3::LteFfrAlgorithm</b><br>
<ul>
<li><b>FrCellTypeId</b>: Downlink FR cell type ID for automatic configuration,default value is 0 and it means that user needs to configure FR algorithm manually,if it is set to 1,2 or 3 FR algorithm will be configured automatically</li>
<li><b>EnabledInUplink</b>: If FR algorithm will also work in Uplink, default value true</li>
</ul>
<b>ns3::LteFrHardAlgorithm</b><br>
<ul>
<li><b>UlSubBandOffset</b>: Uplink Offset in number of Resource Block Groups</li>
<li><b>UlSubBandwidth</b>: Uplink Transmission SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlSubBandOffset</b>: Downlink Offset in number of Resource Block Groups</li>
<li><b>DlSubBandwidth</b>: Downlink Transmission SubBandwidth Configuration in number of Resource Block Groups</li>
</ul>
<b>ns3::LteFrStrictAlgorithm</b><br>
<ul>
<li><b>UlCommonSubBandwidth</b>: Uplink Common SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>UlEdgeSubBandOffset</b>: Uplink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>UlEdgeSubBandwidth</b>: Uplink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlCommonSubBandwidth</b>: Downlink Common SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandOffset</b>: Downlink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandwidth</b>: Downlink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>RsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Edge sub-band</li>
<li><b>CenterPowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>EdgePowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>CenterAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in center areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>EdgeAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in edge areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
</ul>
<b>ns3::LteFrSoftAlgorithm</b><br>
<ul>
<li><b>UlEdgeSubBandOffset</b>: Uplink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>UlEdgeSubBandwidth</b>: Uplink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandOffset</b>: Downlink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandwidth</b>: Downlink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>AllowCenterUeUseEdgeSubBand</b>: If true center UEs can receive on Edge SubBand RBGs</li>
<li><b>RsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Edge sub-band</li>
<li><b>CenterPowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>EdgePowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>CenterAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in center areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>EdgeAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in edge areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
</ul>
<b>ns3::LteFfrSoftAlgorithm</b><br>
<ul>
<li><b>UlCommonSubBandwidth</b>: Uplink Medium (Common) SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>UlEdgeSubBandOffset</b>: Uplink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>UlEdgeSubBandwidth</b>: Uplink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlCommonSubBandwidth</b>: Downlink Medium (Common) SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandOffset</b>: Downlink Edge SubBand Offset in number of Resource Block Groups</li>
<li><b>DlEdgeSubBandwidth</b>: Downlink Edge SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>CenterRsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Medium sub-band</li>
<li><b>EdgeRsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Edge sub-band</li>
<li><b>CenterAreaPowerOffset</b>: PdschConfigDedicated::Pa value for Center Sub-band, default value dB0</li>
<li><b>MediumAreaPowerOffset</b>: PdschConfigDedicated::Pa value for Medium Sub-band, default value dB0</li>
<li><b>EdgeAreaPowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>CenterAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in center areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>MediumAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in medium areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>EdgeAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in edge areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
</ul>
<b>ns3::LteFfrEnhancedAlgorithm</b><br>
<ul>
<li><b>UlSubBandOffset</b>: Uplink SubBand Offset for this cell in number of Resource Block Groups</li>
<li><b>UlReuse3SubBandwidth</b>: Uplink Reuse 3 SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>UlReuse1SubBandwidth</b>: Uplink Reuse 1 SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlSubBandOffset</b>: Downlink SubBand Offset for this cell in number of Resource Block Groups</li>
<li><b>DlReuse3SubBandwidth</b>: Downlink Reuse 3 SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>DlReuse1SubBandwidth</b>: Downlink Reuse 1 SubBandwidth Configuration in number of Resource Block Groups</li>
<li><b>RsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Edge sub-band</li>
<li><b>CenterAreaPowerOffset</b>: PdschConfigDedicated::Pa value for Center Sub-band, default value dB0</li>
<li><b>EdgeAreaPowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>DlCqiThreshold</b>: If the DL-CQI for RBG of is higher than this threshold, transmission on RBG is possible</li>
<li><b>UlCqiThreshold</b>: If the UL-CQI for RBG of is higher than this threshold, transmission on RBG is possible</li>
<li><b>CenterAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in center areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>EdgeAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in edge areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
</ul>
<b>ns3::LteFfrDistributedAlgorithm</b><br>
<ul>
<li><b>CalculationInterval</b>: Time interval between calculation of Edge sub-band, Default value 1 second</li>
<li><b>RsrqThreshold</b>: If the RSRQ of is worse than this threshold, UE should be served in Edge sub-band</li>
<li><b>RsrpDifferenceThreshold</b>: If the difference between the power of the signal received by UE from the serving cell and the power of the signal received from the adjacent cell is less than a RsrpDifferenceThreshold value, the cell weight is incremented</li>
<li><b>CenterPowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>EdgePowerOffset</b>: PdschConfigDedicated::Pa value for Edge Sub-band, default value dB0</li>
<li><b>EdgeRbNum</b>: Number of RB that can be used in edge Sub-band</li>
<li><b>CenterAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in center areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
<li><b>EdgeAreaTpc</b>: TPC value which will be set in DL-DCI for UEs in edge areaAbsolute mode is used, default value 1 is mapped to -1 according toTS36.213 Table 5.1.1.1-2</li>
</ul>
<b>ns3::LteUePowerControl</b><br>
<ul>
<li><b>ClosedLoop</b>: If true Closed Loop mode will be active, otherwise Open Loop</li>
<li><b>AccumulationEnabled</b>: If true TCP accumulation mode will be active, otherwise absolute mode will be active</li>
<li><b>Alpha</b>: Value of Alpha paramter</li>
<li><b>Pcmax</b>: Max Transmission power in dBm, Default value 23 dBmTS36.101 section 6.2.3</li>
<li><b>Pcmin</b>: Min Transmission power in dBm, Default value -40 dBmTS36.101 section 6.2.3</li>
<li><b>PoNominalPusch</b>: P_O_NOMINAL_PUSCH   INT (-126 ... 24), Default value -80</li>
<li><b>PoUePusch</b>: P_O_UE_PUSCH   INT(-8...7), Default value 0</li>
<li><b>PsrsOffset</b>: P_SRS_OFFSET   INT(0...15), Default value 7</li>
</ul>
<b>ns3::YansWifiPhy</b><br>
<ul>
<li><b>EnergyDetectionThreshold</b>: The energy of a received signal should be higher than this threshold (dbm) to allow the PHY layer to detect the signal.</li>
<li><b>CcaMode1Threshold</b>: The energy of a received signal should be higher than this threshold (dbm) to allow the PHY layer to declare CCA BUSY state</li>
<li><b>TxGain</b>: Transmission gain (dB).</li>
<li><b>RxGain</b>: Reception gain (dB).</li>
<li><b>TxPowerLevels</b>: Number of transmission power levels available between TxPowerStart and TxPowerEnd included.</li>
<li><b>TxPowerEnd</b>: Maximum available transmission level (dbm).</li>
<li><b>TxPowerStart</b>: Minimum available transmission level (dbm).</li>
<li><b>RxNoiseFigure</b>: Loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver. According to Wikipedia (http://en.wikipedia.org/wiki/Noise_figure), this is "the difference in decibels (dB) between the noise output of the actual receiver to the noise output of an  ideal receiver with the same overall gain and bandwidth when the receivers  are connected to sources at the standard noise temperature T0 (usually 290 K)". For</li>
<li><b>State</b>: The state of the PHY layer</li>
<li><b>ChannelSwitchDelay</b>: Delay between two short frames transmitted on different frequencies.</li>
<li><b>ChannelNumber</b>: Channel center frequency = Channel starting frequency + 5 MHz * nch</li>
<li><b>Frequency</b>: The operating frequency.</li>
<li><b>Transmitters</b>: The number of transmitters.</li>
<li><b>Recievers</b>: The number of recievers.</li>
<li><b>ShortGuardEnabled</b>: Whether or not short guard interval is enabled.</li>
<li><b>LdpcEnabled</b>: Whether or not LDPC is enabled.</li>
<li><b>STBCEnabled</b>: Whether or not STBC is enabled.</li>
<li><b>GreenfieldEnabled</b>: Whether or not STBC is enabled.</li>
<li><b>ChannelBonding</b>: Whether 20MHz or 40MHz.</li>
</ul>
<b>ns3::YansWifiChannel</b><br>
<ul>
<li><b>PropagationLossModel</b>: A pointer to the propagation loss model attached to this channel.</li>
<li><b>PropagationDelayModel</b>: A pointer to the propagation delay model attached to this channel.</li>
</ul>
<b>ns3::WifiMacQueue</b><br>
<ul>
<li><b>MaxPacketNumber</b>: If a packet arrives when there are already this number of packets, it is dropped.</li>
<li><b>MaxDelay</b>: If a packet stays longer than this delay in the queue, it is dropped.</li>
</ul>
<b>ns3::DcaTxop</b><br>
<ul>
<li><b>Queue</b>: The WifiMacQueue object</li>
</ul>
<b>ns3::Dcf</b><br>
<ul>
<li><b>MinCw</b>: The minimum value of the contention window.</li>
<li><b>MaxCw</b>: The maximum value of the contention window.</li>
<li><b>Aifsn</b>: The AIFSN: the default value conforms to simple DCA.</li>
</ul>
<b>ns3::WifiMac</b><br>
<ul>
<li><b>CtsTimeout</b>: When this timeout expires, the RTS/CTS handshake has failed.</li>
<li><b>AckTimeout</b>: When this timeout expires, the DATA/ACK handshake has failed.</li>
<li><b>BasicBlockAckTimeout</b>: When this timeout expires, the BASIC_BLOCK_ACK_REQ/BASIC_BLOCK_ACK handshake has failed.</li>
<li><b>CompressedBlockAckTimeout</b>: When this timeout expires, the COMPRESSED_BLOCK_ACK_REQ/COMPRESSED_BLOCK_ACK handshake has failed.</li>
<li><b>Sifs</b>: The value of the SIFS constant.</li>
<li><b>EifsNoDifs</b>: The value of EIFS-DIFS</li>
<li><b>Slot</b>: The duration of a Slot.</li>
<li><b>Pifs</b>: The value of the PIFS constant.</li>
<li><b>Rifs</b>: The value of the RIFS constant.</li>
<li><b>MaxPropagationDelay</b>: The maximum propagation delay. Unused for now.</li>
<li><b>Ssid</b>: The ssid we want to belong to.</li>
</ul>
<b>ns3::RegularWifiMac</b><br>
<ul>
<li><b>QosSupported</b>: This Boolean attribute is set to enable 802.11e/WMM-style QoS support at this STA</li>
<li><b>HtSupported</b>: This Boolean attribute is set to enable 802.11n support at this STA</li>
<li><b>CtsToSelfSupported</b>: Use CTS to Self when using a rate that is not in the basic set rate</li>
<li><b>DcaTxop</b>: The DcaTxop object</li>
<li><b>VO_EdcaTxopN</b>: Queue that manages packets belonging to AC_VO access class</li>
<li><b>VI_EdcaTxopN</b>: Queue that manages packets belonging to AC_VI access class</li>
<li><b>BE_EdcaTxopN</b>: Queue that manages packets belonging to AC_BE access class</li>
<li><b>BK_EdcaTxopN</b>: Queue that manages packets belonging to AC_BK access class</li>
</ul>
<b>ns3::WifiRemoteStationManager</b><br>
<ul>
<li><b>IsLowLatency</b>: If true, we attempt to modelize a so-called low-latency device: a device where decisions about tx parameters can be made on a per-packet basis and feedback about the transmission of each packet is obtained before sending the next. Otherwise, we modelize a  high-latency device, that is a device where we cannot update our decision about tx parameters after every packet transmission.</li>
<li><b>MaxSsrc</b>: The maximum number of retransmission attempts for an RTS. This value will not have any effect on some rate control algorithms.</li>
<li><b>MaxSlrc</b>: The maximum number of retransmission attempts for a DATA packet. This value will not have any effect on some rate control algorithms.</li>
<li><b>RtsCtsThreshold</b>: If  the size of the data packet + LLC header + MAC header + FCS trailer is bigger than this value, we use an RTS/CTS handshake before sending the data, as per IEEE Std. 802.11-2012, Section 9.3.5. This value will not have any effect on some rate control algorithms.</li>
<li><b>FragmentationThreshold</b>: If the size of the data packet + LLC header + MAC header + FCS trailer is biggerthan this value, we fragment it such that the size of the fragments are equal or smaller than this value, as per IEEE Std. 802.11-2012, Section 9.5. This value will not have any effect on some rate control algorithms.</li>
<li><b>NonUnicastMode</b>: Wifi mode used for non-unicast transmissions.</li>
<li><b>DefaultTxPowerLevel</b>: Default power level to be used for transmissions. This is the power level that is used by all those WifiManagers that do notimplement TX power control.</li>
</ul>
<b>ns3::ApWifiMac</b><br>
<ul>
<li><b>BeaconInterval</b>: Delay between two beacons</li>
<li><b>BeaconJitter</b>: A uniform random variable to cause the initial beacon starting time (after simulation time 0) to be distributed between 0 and the BeaconInterval.</li>
<li><b>EnableBeaconJitter</b>: If beacons are enabled, whether to jitter the initial send event.</li>
<li><b>BeaconGeneration</b>: Whether or not beacons are generated.</li>
</ul>
<b>ns3::StaWifiMac</b><br>
<ul>
<li><b>ProbeRequestTimeout</b>: The interval between two consecutive probe request attempts.</li>
<li><b>AssocRequestTimeout</b>: The interval between two consecutive assoc request attempts.</li>
<li><b>MaxMissedBeacons</b>: Number of beacons which much be consecutively missed before we attempt to restart association.</li>
<li><b>ActiveProbing</b>: If true, we send probe requests. If false, we don't. NOTE: if more than one STA in your simulation is using active probing, you should enable it at a different simulation time for each STA, otherwise all the STAs will start sending probes at the same time resulting in collisions. See bug 1060 for more info.</li>
</ul>
<b>ns3::WifiNetDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>Channel</b>: The channel attached to this device</li>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
<li><b>Mac</b>: The MAC layer attached to this device.</li>
<li><b>RemoteStationManager</b>: The station manager attached to this device.</li>
</ul>
<b>ns3::ArfWifiManager</b><br>
<ul>
<li><b>TimerThreshold</b>: The 'timer' threshold in the ARF algorithm.</li>
<li><b>SuccessThreshold</b>: The minimum number of sucessfull transmissions to try a new rate.</li>
</ul>
<b>ns3::AarfWifiManager</b><br>
<ul>
<li><b>SuccessK</b>: Multiplication factor for the success threshold in the AARF algorithm.</li>
<li><b>TimerK</b>: Multiplication factor for the timer threshold in the AARF algorithm.</li>
<li><b>MaxSuccessThreshold</b>: Maximum value of the success threshold in the AARF algorithm.</li>
<li><b>MinTimerThreshold</b>: The minimum value for the 'timer' threshold in the AARF algorithm.</li>
<li><b>MinSuccessThreshold</b>: The minimum value for the success threshold in the AARF algorithm.</li>
</ul>
<b>ns3::IdealWifiManager</b><br>
<ul>
<li><b>BerThreshold</b>: The maximum Bit Error Rate acceptable at any transmission mode</li>
</ul>
<b>ns3::ConstantRateWifiManager</b><br>
<ul>
<li><b>DataMode</b>: The transmission mode to use for every data packet transmission</li>
<li><b>ControlMode</b>: The transmission mode to use for every control packet transmission.</li>
</ul>
<b>ns3::AmrrWifiManager</b><br>
<ul>
<li><b>UpdatePeriod</b>: The interval between decisions about rate control changes</li>
<li><b>FailureRatio</b>: Ratio of minimum erroneous transmissions needed to switch to a lower rate</li>
<li><b>SuccessRatio</b>: Ratio of maximum erroneous transmissions needed to switch to a higher rate</li>
<li><b>MaxSuccessThreshold</b>: Maximum number of consecutive success periods needed to switch to a higher rate</li>
<li><b>MinSuccessThreshold</b>: Minimum number of consecutive success periods needed to switch to a higher rate</li>
</ul>
<b>ns3::OnoeWifiManager</b><br>
<ul>
<li><b>UpdatePeriod</b>: The interval between decisions about rate control changes</li>
<li><b>RaiseThreshold</b>: Attempt to raise the rate if we hit that threshold</li>
<li><b>AddCreditThreshold</b>: Add credit threshold</li>
</ul>
<b>ns3::RraaWifiManager</b><br>
<ul>
<li><b>Basic</b>: If true the RRAA-BASIC algorithm will be used, otherwise the RRAA wil be used</li>
<li><b>Timeout</b>: Timeout for the RRAA BASIC loss estimaton block (s)</li>
<li><b>ewndFor54mbps</b>: ewnd parameter for 54 Mbs data mode</li>
<li><b>ewndFor48mbps</b>: ewnd parameter for 48 Mbs data mode</li>
<li><b>ewndFor36mbps</b>: ewnd parameter for 36 Mbs data mode</li>
<li><b>ewndFor24mbps</b>: ewnd parameter for 24 Mbs data mode</li>
<li><b>ewndFor18mbps</b>: ewnd parameter for 18 Mbs data mode</li>
<li><b>ewndFor12mbps</b>: ewnd parameter for 12 Mbs data mode</li>
<li><b>ewndFor9mbps</b>: ewnd parameter for 9 Mbs data mode</li>
<li><b>ewndFor6mbps</b>: ewnd parameter for 6 Mbs data mode</li>
<li><b>poriFor48mbps</b>: Pori parameter for 48 Mbs data mode</li>
<li><b>poriFor36mbps</b>: Pori parameter for 36 Mbs data mode</li>
<li><b>poriFor24mbps</b>: Pori parameter for 24 Mbs data mode</li>
<li><b>poriFor18mbps</b>: Pori parameter for 18 Mbs data mode</li>
<li><b>poriFor12mbps</b>: Pori parameter for 12 Mbs data mode</li>
<li><b>poriFor9mbps</b>: Pori parameter for 9 Mbs data mode</li>
<li><b>poriFor6mbps</b>: Pori parameter for 6 Mbs data mode</li>
<li><b>pmtlFor54mbps</b>: Pmtl parameter for 54 Mbs data mode</li>
<li><b>pmtlFor48mbps</b>: Pmtl parameter for 48 Mbs data mode</li>
<li><b>pmtlFor36mbps</b>: Pmtl parameter for 36 Mbs data mode</li>
<li><b>pmtlFor24mbps</b>: Pmtl parameter for 24 Mbs data mode</li>
<li><b>pmtlFor18mbps</b>: Pmtl parameter for 18 Mbs data mode</li>
<li><b>pmtlFor12mbps</b>: Pmtl parameter for 12 Mbs data mode</li>
<li><b>pmtlFor9mbps</b>: Pmtl parameter for 9 Mbs data mode</li>
</ul>
<b>ns3::AarfcdWifiManager</b><br>
<ul>
<li><b>SuccessK</b>: Multiplication factor for the success threshold in the AARF algorithm.</li>
<li><b>TimerK</b>: Multiplication factor for the timer threshold in the AARF algorithm.</li>
<li><b>MaxSuccessThreshold</b>: Maximum value of the success threshold in the AARF algorithm.</li>
<li><b>MinTimerThreshold</b>: The minimum value for the 'timer' threshold in the AARF algorithm.</li>
<li><b>MinSuccessThreshold</b>: The minimum value for the success threshold in the AARF algorithm.</li>
<li><b>MinRtsWnd</b>: Minimum value for Rts window of Aarf-CD</li>
<li><b>MaxRtsWnd</b>: Maximum value for Rts window of Aarf-CD</li>
<li><b>TurnOffRtsAfterRateDecrease</b>: If true the RTS mechanism will be turned off when the rate will be decreased</li>
<li><b>TurnOnRtsAfterRateIncrease</b>: If true the RTS mechanism will be turned on when the rate will be increased</li>
</ul>
<b>ns3::CaraWifiManager</b><br>
<ul>
<li><b>ProbeThreshold</b>: The number of consecutive transmissions failure to activate the RTS probe.</li>
<li><b>FailureThreshold</b>: The number of consecutive transmissions failure to decrease the rate.</li>
<li><b>SuccessThreshold</b>: The minimum number of sucessfull transmissions to try a new rate.</li>
<li><b>Timeout</b>: The 'timer' in the CARA algorithm</li>
</ul>
<b>ns3::MinstrelWifiManager</b><br>
<ul>
<li><b>UpdateStatistics</b>: The interval between updating statistics table </li>
<li><b>LookAroundRate</b>: the percentage to try other rates</li>
<li><b>EWMA</b>: EWMA level</li>
<li><b>SampleColumn</b>: The number of columns used for sampling</li>
<li><b>PacketLength</b>: The packet length used for calculating mode TxTime</li>
</ul>
<b>ns3::QosTag</b><br>
<ul>
<li><b>tid</b>: The tid that indicates AC which packet belongs</li>
</ul>
<b>ns3::EdcaTxopN</b><br>
<ul>
<li><b>BlockAckThreshold</b>: If number of packets in this queue reaches this value,                                         block ack mechanism is used. If this value is 0, block ack is never used.</li>
<li><b>BlockAckInactivityTimeout</b>: Represents max time (blocks of 1024 micro seconds) allowed for block ack                                                 inactivity. If this value isn't equal to 0 a timer start after that a                                                 block ack setup is completed and will be reset every time that a block                                                 ack frame is received. If this value is 0, block ack inactivity timeout won't be used.</li>
<li><b>Queue</b>: The WifiMacQueue object</li>
</ul>
<b>ns3::MsduStandardAggregator</b><br>
<ul>
<li><b>MaxAmsduSize</b>: Max length in byte of an A-MSDU</li>
</ul>
<b>ns3::SnrTag</b><br>
<ul>
<li><b>Snr</b>: The snr of the last packet received</li>
</ul>
<b>ns3::AthstatsWifiTraceSink</b><br>
<ul>
<li><b>Interval</b>: Time interval between reports</li>
</ul>
<b>ns3::WifiRadioEnergyModel</b><br>
<ul>
<li><b>IdleCurrentA</b>: The default radio Idle current in Ampere.</li>
<li><b>CcaBusyCurrentA</b>: The default radio CCA Busy State current in Ampere.</li>
<li><b>TxCurrentA</b>: The radio Tx current in Ampere.</li>
<li><b>RxCurrentA</b>: The radio Rx current in Ampere.</li>
<li><b>SwitchingCurrentA</b>: The default radio Channel Switch current in Ampere.</li>
<li><b>SleepCurrentA</b>: The radio Sleep current in Ampere.</li>
<li><b>TxCurrentModel</b>: A pointer to the attached tx current model.</li>
</ul>
<b>ns3::BasicEnergySource</b><br>
<ul>
<li><b>BasicEnergySourceInitialEnergyJ</b>: Initial energy stored in basic energy source.</li>
<li><b>BasicEnergySupplyVoltageV</b>: Initial supply voltage for basic energy source.</li>
<li><b>BasicEnergyLowBatteryThreshold</b>: Low battery threshold for basic energy source.</li>
<li><b>BasicEnergyHighBatteryThreshold</b>: High battery threshold for basic energy source.</li>
<li><b>PeriodicEnergyUpdateInterval</b>: Time between two consecutive periodic energy updates.</li>
</ul>
<b>ns3::LiIonEnergySource</b><br>
<ul>
<li><b>LiIonEnergySourceInitialEnergyJ</b>: Initial energy stored in basic energy source.</li>
<li><b>LiIonEnergyLowBatteryThreshold</b>: Low battery threshold for LiIon energy source.</li>
<li><b>InitialCellVoltage</b>: Initial (maximum) voltage of the cell (fully charged).</li>
<li><b>NominalCellVoltage</b>: Nominal voltage of the cell.</li>
<li><b>ExpCellVoltage</b>: Cell voltage at the end of the exponential zone.</li>
<li><b>RatedCapacity</b>: Rated capacity of the cell.</li>
<li><b>NomCapacity</b>: Cell capacity at the end of the nominal zone.</li>
<li><b>ExpCapacity</b>: Cell Capacity at the end of the exponential zone.</li>
<li><b>InternalResistance</b>: Internal resistance of the cell</li>
<li><b>TypCurrent</b>: Typical discharge current used to fit the curves</li>
<li><b>ThresholdVoltage</b>: Minimum threshold voltage to consider the battery depleted.</li>
<li><b>PeriodicEnergyUpdateInterval</b>: Time between two consecutive periodic energy updates.</li>
</ul>
<b>ns3::RvBatteryModel</b><br>
<ul>
<li><b>RvBatteryModelPeriodicEnergyUpdateInterval</b>: RV battery model sampling interval.</li>
<li><b>RvBatteryModelLowBatteryThreshold</b>: Low battery threshold.</li>
<li><b>RvBatteryModelOpenCircuitVoltage</b>: RV battery model open circuit voltage.</li>
<li><b>RvBatteryModelCutoffVoltage</b>: RV battery model cutoff voltage.</li>
<li><b>RvBatteryModelAlphaValue</b>: RV battery model alpha value.</li>
<li><b>RvBatteryModelBetaValue</b>: RV battery model beta value.</li>
<li><b>RvBatteryModelNumOfTerms</b>: The number of terms of the infinite sum for estimating battery level.</li>
</ul>
<b>ns3::BasicEnergyHarvester</b><br>
<ul>
<li><b>PeriodicHarvestedPowerUpdateInterval</b>: Time between two consecutive periodic updates of the harvested power. By default, the value is updated every 1 s</li>
<li><b>HarvestablePower</b>: The harvestable power [Watts] that the energy harvester is allowed to harvest. By default, the model will allow to harvest an amount of power defined by a uniformly distributed random variable in 0 and 2.0 Watts</li>
</ul>
<b>ns3::LinearWifiTxCurrentModel</b><br>
<ul>
<li><b>Eta</b>: The efficiency of the power amplifier.</li>
<li><b>Voltage</b>: The supply voltage (in Volts).</li>
<li><b>IdleCurrent</b>: The current in the IDLE state (in Watts).</li>
</ul>
<b>ns3::UanChannel</b><br>
<ul>
<li><b>PropagationModel</b>: A pointer to the propagation model.</li>
<li><b>NoiseModel</b>: A pointer to the model of the channel ambient noise.</li>
</ul>
<b>ns3::UanNoiseModelDefault</b><br>
<ul>
<li><b>Wind</b>: Wind speed in m/s.</li>
<li><b>Shipping</b>: Shipping contribution to noise between 0 and 1.</li>
</ul>
<b>ns3::UanPhyGen</b><br>
<ul>
<li><b>CcaThreshold</b>: Aggregate energy of incoming signals to move to CCA Busy state dB.</li>
<li><b>RxThreshold</b>: Required SNR for signal acquisition in dB.</li>
<li><b>TxPower</b>: Transmission output power in dB.</li>
<li><b>RxGain</b>: Gain added to incoming signal at receiver.</li>
<li><b>SupportedModes</b>: List of modes supported by this PHY.</li>
<li><b>PerModel</b>: Functor to calculate PER based on SINR and TxMode.</li>
<li><b>SinrModel</b>: Functor to calculate SINR based on pkt arrivals and modes.</li>
</ul>
<b>ns3::UanPhyPerGenDefault</b><br>
<ul>
<li><b>Threshold</b>: SINR cutoff for good packet reception.</li>
</ul>
<b>ns3::UanPhyCalcSinrFhFsk</b><br>
<ul>
<li><b>NumberOfHops</b>: Number of frequencies in hopping pattern.</li>
</ul>
<b>ns3::UanNetDevice</b><br>
<ul>
<li><b>Channel</b>: The channel attached to this device.</li>
<li><b>Phy</b>: The PHY layer attached to this device.</li>
<li><b>Mac</b>: The MAC layer attached to this device.</li>
<li><b>Transducer</b>: The Transducer attached to this device.</li>
</ul>
<b>ns3::UanMacCw</b><br>
<ul>
<li><b>CW</b>: The MAC parameter CW.</li>
<li><b>SlotTime</b>: Time slot duration for MAC backoff.</li>
</ul>
<b>ns3::UanPropModelThorp</b><br>
<ul>
<li><b>SpreadCoef</b>: Spreading coefficient used in calculation of Thorp's approximation.</li>
</ul>
<b>ns3::UanPhyDual</b><br>
<ul>
<li><b>CcaThresholdPhy1</b>: Aggregate energy of incoming signals to move to CCA Busy state dB of Phy1.</li>
<li><b>CcaThresholdPhy2</b>: Aggregate energy of incoming signals to move to CCA Busy state dB of Phy2.</li>
<li><b>TxPowerPhy1</b>: Transmission output power in dB of Phy1.</li>
<li><b>TxPowerPhy2</b>: Transmission output power in dB of Phy2.</li>
<li><b>RxGainPhy1</b>: Gain added to incoming signal at receiver of Phy1.</li>
<li><b>RxGainPhy2</b>: Gain added to incoming signal at receiver of Phy2.</li>
<li><b>SupportedModesPhy1</b>: List of modes supported by Phy1.</li>
<li><b>SupportedModesPhy2</b>: List of modes supported by Phy2.</li>
<li><b>PerModelPhy1</b>: Functor to calculate PER based on SINR and TxMode for Phy1.</li>
<li><b>PerModelPhy2</b>: Functor to calculate PER based on SINR and TxMode for Phy2.</li>
<li><b>SinrModelPhy1</b>: Functor to calculate SINR based on pkt arrivals and modes for Phy1.</li>
<li><b>SinrModelPhy2</b>: Functor to calculate SINR based on pkt arrivals and modes for Phy2.</li>
</ul>
<b>ns3::UanMacRc</b><br>
<ul>
<li><b>RetryRate</b>: Number of retry attempts per second (of RTS/GWPING).</li>
<li><b>MaxFrames</b>: Maximum number of frames to include in a single RTS.</li>
<li><b>QueueLimit</b>: Maximum packets to queue at MAC.</li>
<li><b>SIFS</b>: Spacing to give between frames (this should match gateway).</li>
<li><b>NumberOfRates</b>: Number of rate divisions supported by each PHY.</li>
<li><b>MinRetryRate</b>: Smallest allowed RTS retry rate.</li>
<li><b>RetryStep</b>: Retry rate increment.</li>
<li><b>MaxPropDelay</b>: Maximum possible propagation delay to gateway.</li>
</ul>
<b>ns3::UanMacRcGw</b><br>
<ul>
<li><b>MaxReservations</b>: Maximum number of reservations to accept per cycle.</li>
<li><b>NumberOfRates</b>: Number of rates per Phy layer.</li>
<li><b>MaxPropDelay</b>: Maximum propagation delay between gateway and non-gateway nodes.</li>
<li><b>SIFS</b>: Spacing between frames to account for timing error and processing delay.</li>
<li><b>NumberOfNodes</b>: Number of non-gateway nodes in this gateway's neighborhood.</li>
<li><b>MinRetryRate</b>: Smallest allowed RTS retry rate.</li>
<li><b>RetryStep</b>: Retry rate increment.</li>
<li><b>TotalRate</b>: Total available channel rate in bps (for a single channel, without splitting reservation channel).</li>
<li><b>RateStep</b>: Increments available for rate assignment in bps.</li>
<li><b>FrameSize</b>: Size of data frames in bytes.</li>
</ul>
<b>ns3::AcousticModemEnergyModel</b><br>
<ul>
<li><b>TxPowerW</b>: The modem Tx power in Watts</li>
<li><b>RxPowerW</b>: The modem Rx power in Watts</li>
<li><b>IdlePowerW</b>: The modem Idle power in Watts</li>
<li><b>SleepPowerW</b>: The modem Sleep power in Watts</li>
</ul>
<b>ns3::aodv::RoutingProtocol</b><br>
<ul>
<li><b>HelloInterval</b>: HELLO messages emission interval.</li>
<li><b>RreqRetries</b>: Maximum number of retransmissions of RREQ to discover a route</li>
<li><b>RreqRateLimit</b>: Maximum number of RREQ per second.</li>
<li><b>RerrRateLimit</b>: Maximum number of RERR per second.</li>
<li><b>NodeTraversalTime</b>: Conservative estimate of the average one hop traversal time for packets and should include queuing delays, interrupt processing times and transfer times.</li>
<li><b>NextHopWait</b>: Period of our waiting for the neighbour's RREP_ACK = 10 ms + NodeTraversalTime</li>
<li><b>ActiveRouteTimeout</b>: Period of time during which the route is considered to be valid</li>
<li><b>MyRouteTimeout</b>: Value of lifetime field in RREP generating by this node = 2 * max(ActiveRouteTimeout, PathDiscoveryTime)</li>
<li><b>BlackListTimeout</b>: Time for which the node is put into the blacklist = RreqRetries * NetTraversalTime</li>
<li><b>DeletePeriod</b>: DeletePeriod is intended to provide an upper bound on the time for which an upstream node A can have a neighbor B as an active next hop for destination D, while B has invalidated the route to D. = 5 * max (HelloInterval, ActiveRouteTimeout)</li>
<li><b>NetDiameter</b>: Net diameter measures the maximum possible number of hops between two nodes in the network</li>
<li><b>NetTraversalTime</b>: Estimate of the average net traversal time = 2 * NodeTraversalTime * NetDiameter</li>
<li><b>PathDiscoveryTime</b>: Estimate of maximum time needed to find route in network = 2 * NetTraversalTime</li>
<li><b>MaxQueueLen</b>: Maximum number of packets that we allow a routing protocol to buffer.</li>
<li><b>MaxQueueTime</b>: Maximum time packets can be queued (in seconds)</li>
<li><b>AllowedHelloLoss</b>: Number of hello messages which may be loss for valid link.</li>
<li><b>GratuitousReply</b>: Indicates whether a gratuitous RREP should be unicast to the node originated route discovery.</li>
<li><b>DestinationOnly</b>: Indicates only the destination may respond to this RREQ.</li>
<li><b>EnableHello</b>: Indicates whether a hello messages enable.</li>
<li><b>EnableBroadcast</b>: Indicates whether a broadcast data packets forwarding enable.</li>
<li><b>UniformRv</b>: Access to the underlying UniformRandomVariable</li>
</ul>
<b>ns3::MeshPointDevice</b><br>
<ul>
<li><b>Mtu</b>: The MAC-level Maximum Transmission Unit</li>
<li><b>RoutingProtocol</b>: The mesh routing protocol used by this mesh point.</li>
</ul>
<b>ns3::MeshWifiInterfaceMac</b><br>
<ul>
<li><b>BeaconInterval</b>: Beacon Interval</li>
<li><b>RandomStart</b>: Window when beacon generating starts (uniform random) in seconds</li>
<li><b>BeaconGeneration</b>: Enable/Disable Beaconing.</li>
</ul>
<b>ns3::dot11s::PeerLink</b><br>
<ul>
<li><b>RetryTimeout</b>: Retry timeout</li>
<li><b>HoldingTimeout</b>: Holding timeout</li>
<li><b>ConfirmTimeout</b>: Confirm timeout</li>
<li><b>MaxRetries</b>: Maximum number of retries</li>
<li><b>MaxBeaconLoss</b>: Maximum number of lost beacons before link will be closed</li>
<li><b>MaxPacketFailure</b>: Maximum number of failed packets before link will be closed</li>
</ul>
<b>ns3::dot11s::PeerManagementProtocol</b><br>
<ul>
<li><b>MaxNumberOfPeerLinks</b>: Maximum number of peer links</li>
<li><b>MaxBeaconShiftValue</b>: Maximum number of TUs for beacon shifting</li>
<li><b>EnableBeaconCollisionAvoidance</b>: Enable/Disable Beacon collision avoidance.</li>
</ul>
<b>ns3::dot11s::HwmpProtocol</b><br>
<ul>
<li><b>RandomStart</b>: Random delay at first proactive PREQ</li>
<li><b>MaxQueueSize</b>: Maximum number of packets we can store when resolving route</li>
<li><b>Dot11MeshHWMPmaxPREQretries</b>: Maximum number of retries before we suppose the destination to be unreachable</li>
<li><b>Dot11MeshHWMPnetDiameterTraversalTime</b>: Time we suppose the packet to go from one edge of the network to another</li>
<li><b>Dot11MeshHWMPpreqMinInterval</b>: Minimal interval between to successive PREQs</li>
<li><b>Dot11MeshHWMPperrMinInterval</b>: Minimal interval between to successive PREQs</li>
<li><b>Dot11MeshHWMPactiveRootTimeout</b>: Lifetime of poractive routing information</li>
<li><b>Dot11MeshHWMPactivePathTimeout</b>: Lifetime of reactive routing information</li>
<li><b>Dot11MeshHWMPpathToRootInterval</b>: Interval between two successive proactive PREQs</li>
<li><b>Dot11MeshHWMPrannInterval</b>: Lifetime of poractive routing information</li>
<li><b>MaxTtl</b>: Initial value of Time To Live field</li>
<li><b>UnicastPerrThreshold</b>: Maximum number of PERR receivers, when we send a PERR as a chain of unicasts</li>
<li><b>UnicastPreqThreshold</b>: Maximum number of PREQ receivers, when we send a PREQ as a chain of unicasts</li>
<li><b>UnicastDataThreshold</b>: Maximum number ofbroadcast receivers, when we send a broadcast as a chain of unicasts</li>
<li><b>DoFlag</b>: Destination only HWMP flag</li>
<li><b>RfFlag</b>: Reply and forward flag</li>
</ul>
<b>ns3::dot11s::AirtimeLinkMetricCalculator</b><br>
<ul>
<li><b>TestLength</b>: Rate should be estimated using test length.</li>
<li><b>Dot11MetricTid</b>: TID used to calculate metric (data rate)</li>
</ul>
<b>ns3::flame::FlameRtable</b><br>
<ul>
<li><b>Lifetime</b>: The lifetime of the routing enrty</li>
</ul>
<b>ns3::flame::FlameProtocol</b><br>
<ul>
<li><b>BroadcastInterval</b>: How often we must send broadcast packets</li>
<li><b>MaxCost</b>: Cost threshold after which packet will be dropped</li>
</ul>
<b>ns3::Dot11sStack</b><br>
<ul>
<li><b>Root</b>: The MAC address of root mesh point.</li>
</ul>
<b>ns3::VisualSimulatorImpl</b><br>
<ul>
<li><b>SimulatorImplFactory</b>: Factory for the underlying simulator implementation used by the visualizer.</li>
</ul>
<b>ns3::ndn::ConsumerBatches</b><br>
<ul>
<li><b>Batches</b>: Batches to schedule. Should be vector, containing pairs of time and amount</li>
</ul>
<b>ns3::ndn::Consumer</b><br>
<ul>
<li><b>StartSeq</b>: Initial sequence number</li>
<li><b>Prefix</b>: Name of the Interest</li>
<li><b>LifeTime</b>: LifeTime for interest packet</li>
<li><b>RetxTimer</b>: Timeout defining how frequent retransmission timeouts should be checked</li>
</ul>
<b>ns3::ndn::ConsumerCbr</b><br>
<ul>
<li><b>Frequency</b>: Frequency of interest packets</li>
<li><b>Randomize</b>: Type of send time randomization: none (default), uniform, exponential</li>
<li><b>MaxSeq</b>: Maximum sequence number to request</li>
</ul>
<b>ns3::ndn::ConsumerWindow</b><br>
<ul>
<li><b>Window</b>: Initial size of the window</li>
<li><b>PayloadSize</b>: Average size of content object size (to calculate interest generation rate)</li>
<li><b>Size</b>: Amount of data in megabytes to request, relying on PayloadSize parameter (alternative to MaxSeq attribute)</li>
<li><b>MaxSeq</b>: Maximum sequence number to request (alternative to Size attribute, would activate only if Size is -1). The parameter is activated only if Size negative (not set)</li>
<li><b>InitialWindowOnTimeout</b>: Set window to initial value when timeout occurs</li>
</ul>
<b>ns3::ndn::ConsumerZipfMandelbrot</b><br>
<ul>
<li><b>NumberOfContents</b>: Number of the Contents in total</li>
<li><b>q</b>: parameter of improve rank</li>
<li><b>s</b>: parameter of power</li>
</ul>
<b>ns3::ndn::Producer</b><br>
<ul>
<li><b>Prefix</b>: Prefix, for which producer has the data</li>
<li><b>Postfix</b>: Postfix that is added to the output data (e.g., for adding producer-uniqueness)</li>
<li><b>PayloadSize</b>: Virtual payload size for Content packets</li>
<li><b>Freshness</b>: Freshness of data packets, if 0, then unlimited freshness</li>
<li><b>Signature</b>: Fake signature, 0 valid signature (default), other values application-specific</li>
<li><b>KeyLocator</b>: Name to be used for key locator.  If root, then key locator is not used</li>
</ul>
<b>ns3::ndn::cs::Lru</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Random</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Fifo</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lfu</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lru::AggregateStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Random::AggregateStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Fifo::AggregateStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lfu::AggregateStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lru::Freshness</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Random::Freshness</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Fifo::Freshness</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lfu::Freshness</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Probability::Lru</b><br>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Lru</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Probability::Random</b><br>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Random</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Probability::Fifo</b><br>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Fifo</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Probability::Lfu</b><br>
<ul>
<li><b>CacheProbability</b>: Set probability of caching in ContentStore. If 1, every content is cached. If 0, no content is cached.</li>
</ul>
<b>ns3::ndn::cs::ProbabilityImpl::Lfu</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lru::LifetimeStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Random::LifetimeStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Fifo::LifetimeStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::cs::Lfu::LifetimeStats</b><br>
<ul>
<li><b>MaxSize</b>: Set maximum number of entries in ContentStore. If 0, limit is not enforced</li>
</ul>
<b>ns3::ndn::RttEstimator</b><br>
<ul>
<li><b>MaxMultiplier</b>: Maximum RTO Multiplier</li>
<li><b>InitialEstimation</b>: Initial RTT estimation</li>
<li><b>MinRTO</b>: Minimum retransmit timeout value</li>
<li><b>MaxRTO</b>: Maximum retransmit timeout value</li>
</ul>
<b>ns3::ndn::RttMeanDeviation</b><br>
<ul>
<li><b>Gain</b>: Gain used in estimating the RTT (smoothed RTT), must be 0 < Gain < 1</li>
<li><b>Gain2</b>: Gain2 used in estimating the RTT (variance), must be 0 < Gain2 < 1</li>
</ul>
<b>ns3::olsr::RoutingProtocol</b><br>
<ul>
<li><b>HelloInterval</b>: HELLO messages emission interval.</li>
<li><b>TcInterval</b>: TC messages emission interval.</li>
<li><b>MidInterval</b>: MID messages emission interval.  Normally it is equal to TcInterval.</li>
<li><b>HnaInterval</b>: HNA messages emission interval.  Normally it is equal to TcInterval.</li>
<li><b>Willingness</b>: Willingness of a node to carry and forward traffic for other nodes.</li>
</ul>
<b>ns3::FlowMonitor</b><br>
<ul>
<li><b>MaxPerHopDelay</b>: The maximum per-hop delay that should be considered.  Packets still not received after this delay are to be considered lost.</li>
<li><b>StartTime</b>: The time when the monitoring starts.</li>
<li><b>DelayBinWidth</b>: The width used in the delay histogram.</li>
<li><b>JitterBinWidth</b>: The width used in the jitter histogram.</li>
<li><b>PacketSizeBinWidth</b>: The width used in the packetSize histogram.</li>
<li><b>FlowInterruptionsBinWidth</b>: The width used in the flowInterruptions histogram.</li>
<li><b>FlowInterruptionsMinTime</b>: The minimum inter-arrival time that is considered a flow interruption.</li>
</ul>
<b>ns3::dsdv::RoutingProtocol</b><br>
<ul>
<li><b>PeriodicUpdateInterval</b>: Periodic interval between exchange of full routing tables among nodes. </li>
<li><b>SettlingTime</b>: Minimum time an update is to be stored in adv table before sending outin case of change in metric (in seconds)</li>
<li><b>MaxQueueLen</b>: Maximum number of packets that we allow a routing protocol to buffer.</li>
<li><b>MaxQueuedPacketsPerDst</b>: Maximum number of packets that we allow per destination to buffer.</li>
<li><b>MaxQueueTime</b>: Maximum time packets can be queued (in seconds)</li>
<li><b>EnableBuffering</b>: Enables buffering of data packets if no route to destination is available</li>
<li><b>EnableWST</b>: Enables Weighted Settling Time for the updates before advertising</li>
<li><b>Holdtimes</b>: Times the forwarding Interval to purge the route.</li>
<li><b>WeightedFactor</b>: WeightedFactor for the settling time if Weighted Settling Time is enabled</li>
<li><b>EnableRouteAggregation</b>: Enables Weighted Settling Time for the updates before advertising</li>
<li><b>RouteAggregationTime</b>: Time to aggregate updates before sending them out (in seconds)</li>
</ul>
<b>ns3::dsr::DsrRouting</b><br>
<ul>
<li><b>RouteCache</b>: The route cache for saving routes from route discovery process.</li>
<li><b>RreqTable</b>: The request table to manage route requests.</li>
<li><b>PassiveBuffer</b>: The passive buffer to manage promisucously received passive ack.</li>
<li><b>MaxSendBuffLen</b>: Maximum number of packets that can be stored in send buffer.</li>
<li><b>MaxSendBuffTime</b>: Maximum time packets can be queued in the send buffer .</li>
<li><b>MaxMaintLen</b>: Maximum number of packets that can be stored in maintenance buffer.</li>
<li><b>MaxMaintTime</b>: Maximum time packets can be queued in maintenance buffer.</li>
<li><b>MaxCacheLen</b>: Maximum number of route entries that can be stored in route cache.</li>
<li><b>RouteCacheTimeout</b>: Maximum time the route cache can be queued in route cache.</li>
<li><b>MaxEntriesEachDst</b>: Maximum number of route entries for a single destination to respond.</li>
<li><b>SendBuffInterval</b>: How often to check send buffer for packet with route.</li>
<li><b>NodeTraversalTime</b>: The time it takes to traverse two neighboring nodes.</li>
<li><b>RreqRetries</b>: Maximum number of retransmissions for request discovery of a route.</li>
<li><b>MaintenanceRetries</b>: Maximum number of retransmissions for data packets from maintenance buffer.</li>
<li><b>RequestTableSize</b>: Maximum number of request entries in the request table, set this as the number of nodes in the simulation.</li>
<li><b>RequestIdSize</b>: Maximum number of request source Ids in the request table.</li>
<li><b>UniqueRequestIdSize</b>: Maximum number of request Ids in the request table for a single destination.</li>
<li><b>NonPropRequestTimeout</b>: The timeout value for non-propagation request.</li>
<li><b>DiscoveryHopLimit</b>: The max discovery hop limit for route requests.</li>
<li><b>MaxSalvageCount</b>: The max salvage count for a single data packet.</li>
<li><b>BlacklistTimeout</b>: The time for a neighbor to stay in blacklist.</li>
<li><b>GratReplyHoldoff</b>: The time for gratuitous reply entry to expire.</li>
<li><b>BroadcastJitter</b>: The jitter time to avoid collision for broadcast packets.</li>
<li><b>LinkAckTimeout</b>: The time a packet in maintenance buffer wait for link acknowledgment.</li>
<li><b>TryLinkAcks</b>: The number of link acknowledgment to use.</li>
<li><b>PassiveAckTimeout</b>: The time a packet in maintenance buffer wait for passive acknowledgment.</li>
<li><b>TryPassiveAcks</b>: The number of passive acknowledgment to use.</li>
<li><b>RequestPeriod</b>: The base time interval between route requests.</li>
<li><b>MaxRequestPeriod</b>: The max time interval between route requests.</li>
<li><b>GraReplyTableSize</b>: The gratuitous reply table size.</li>
<li><b>CacheType</b>: Use Link Cache or use Path Cache</li>
<li><b>StabilityDecrFactor</b>: The stability decrease factor for link cache</li>
<li><b>StabilityIncrFactor</b>: The stability increase factor for link cache</li>
<li><b>InitStability</b>: The initial stability factor for link cache</li>
<li><b>MinLifeTime</b>: The minimal life time for link cache</li>
<li><b>UseExtends</b>: The extension time for link cache</li>
<li><b>EnableSubRoute</b>: Enables saving of sub route when receiving route error messages, only available when using path route cache</li>
<li><b>RetransIncr</b>: The increase time for retransmission timer when facing network congestion</li>
<li><b>MaxNetworkQueueSize</b>: The max number of packet to save in the network queue.</li>
<li><b>MaxNetworkQueueDelay</b>: The max time for a packet to stay in the network queue.</li>
<li><b>NumPriorityQueues</b>: The max number of packet to save in the network queue.</li>
<li><b>LinkAcknowledgment</b>: Enable Link layer acknowledgment mechanism</li>
</ul>
<b>ns3::dsr::DsrOptions</b><br>
<ul>
<li><b>OptionNumber</b>: The Dsr option number.</li>
</ul>
<b>ns3::SixLowPanNetDevice</b><br>
<ul>
<li><b>Rfc6282</b>: Use RFC6282 (IPHC) if true, RFC4944 (HC1) otherwise.</li>
<li><b>OmitUdpChecksum</b>: Omit the UDP checksum in IPHC compression.</li>
<li><b>FragmentReassemblyListSize</b>: The maximum size of the reassembly buffer (in packets). Zero meaning infinite.</li>
<li><b>FragmentExpirationTimeout</b>: When this timeout expires, the fragments will be cleared from the buffer.</li>
<li><b>CompressionThreshold</b>: The minimum MAC layer payload size.</li>
<li><b>ForceEtherType</b>: Force a specific EtherType in L2 frames.</li>
<li><b>EtherType</b>: The specific EtherType to be used in L2 frames.</li>
</ul>
<b>ns3::FdNetDevice</b><br>
<ul>
<li><b>Address</b>: The MAC address of this device.</li>
<li><b>Start</b>: The simulation time at which to spin up the device thread.</li>
<li><b>Stop</b>: The simulation time at which to tear down the device thread.</li>
<li><b>EncapsulationMode</b>: The link-layer encapsulation type to use.</li>
<li><b>RxQueueSize</b>: Maximum size of the read queue.  This value limits number of packets that have been read from the network into a memory buffer but have not yet been processed by the simulator.</li>
</ul>
<b>ns3::LteDataRadioBearerInfo</b><br>
<ul>
<li><b>DrbIdentity</b>: The id of this Data Radio Bearer</li>
<li><b>EpsBearerIdentity</b>: The id of the EPS bearer corresponding to this Data Radio Bearer</li>
<li><b>logicalChannelIdentity</b>: The id of the Logical Channel corresponding to this Data Radio Bearer</li>
<li><b>LteRlc</b>: RLC instance of the radio bearer.</li>
<li><b>LtePdcp</b>: PDCP instance of the radio bearer.</li>
</ul>
<b>ns3::LteSignalingRadioBearerInfo</b><br>
<ul>
<li><b>SrbIdentity</b>: The id of this Signaling Radio Bearer</li>
<li><b>LteRlc</b>: RLC instance of the radio bearer.</li>
<li><b>LtePdcp</b>: PDCP instance of the radio bearer.</li>
</ul>
*/

/*!
\page GlobalValueList All GlobalValues

<ul>
  <li><b>\anchor GlobalValueSimulatorImplementationType SimulatorImplementationType</b>: The object class to use as the simulator implementation.  Default value: ns3::DefaultSimulatorImpl.</li>
  <li><b>\anchor GlobalValueSchedulerType SchedulerType</b>: The object class to use as the scheduler implementation.  Default value: ns3::MapScheduler.</li>
  <li><b>\anchor GlobalValueRngSeed RngSeed</b>: The global seed of all rng streams.  Default value: 1.</li>
  <li><b>\anchor GlobalValueRngRun RngRun</b>: The run number used to modify the global seed.  Default value: 1.</li>
  <li><b>\anchor GlobalValueChecksumEnabled ChecksumEnabled</b>: A global switch to enable all checksums for all protocols.  Default value: false.</li>
</ul>
*/

/*!
\page LogComponentList All LogComponents

<ul>
  <li><b>A2A4RsrqHandoverAlgorithm</b></li>
  <li><b>A3RsrpHandoverAlgorithm</b></li>
  <li><b>AarfWifiManager</b></li>
  <li><b>Aarfcd</b></li>
  <li><b>AcousticModemEnergyModel</b></li>
  <li><b>Address</b></li>
  <li><b>AddressUtils</b></li>
  <li><b>AdhocAlohaNoackIdealPhyHelper</b></li>
  <li><b>AdhocWifiMac</b></li>
  <li><b>AlohaNoackMacHeader</b></li>
  <li><b>AlohaNoackNetDevice</b></li>
  <li><b>AmrrWifiRemoteStation</b></li>
  <li><b>AmsduSubframeHeader</b></li>
  <li><b>Angles</b></li>
  <li><b>AnimationInterface</b></li>
  <li><b>AnnotatedTopologyReader</b></li>
  <li><b>AntennaModel</b></li>
  <li><b>AodvNeighbors</b></li>
  <li><b>AodvRequestQueue</b></li>
  <li><b>AodvRoutingProtocol</b></li>
  <li><b>AodvRoutingTable</b></li>
  <li><b>ApWifiMac</b></li>
  <li><b>Application</b></li>
  <li><b>ApplicationPacketProbe</b></li>
  <li><b>ArpCache</b></li>
  <li><b>ArpHeader</b></li>
  <li><b>ArpL3Protocol</b></li>
  <li><b>Asn1Header</b></li>
  <li><b>Athstats</b></li>
  <li><b>AttributeConstructionList</b></li>
  <li><b>AttributeIterator</b></li>
  <li><b>AttributeValue</b></li>
  <li><b>BSLinkManager</b></li>
  <li><b>BSScheduler</b></li>
  <li><b>BSSchedulerRtps</b></li>
  <li><b>BSSchedulerSimple</b></li>
  <li><b>Backoff</b></li>
  <li><b>BandwidthManager</b></li>
  <li><b>BaseStationNetDevice</b></li>
  <li><b>BasicEnergyHarvester</b></li>
  <li><b>BasicEnergySource</b></li>
  <li><b>BlockAckAgreement</b></li>
  <li><b>BlockAckCache</b></li>
  <li><b>BlockAckManager</b></li>
  <li><b>Boolean</b></li>
  <li><b>BooleanProbe</b></li>
  <li><b>Breakpoint</b></li>
  <li><b>BridgeChannel</b></li>
  <li><b>BridgeHelper</b></li>
  <li><b>BridgeNetDevice</b></li>
  <li><b>BsServiceFlowManager</b></li>
  <li><b>Buffer</b></li>
  <li><b>Building</b></li>
  <li><b>BuildingAllocator</b></li>
  <li><b>BuildingList</b></li>
  <li><b>BuildingPositionAllocator</b></li>
  <li><b>BuildingsHelper</b></li>
  <li><b>BuildingsPropagationLossModel</b></li>
  <li><b>BulkSendApplication</b></li>
  <li><b>BurstProfileManager</b></li>
  <li><b>ByteTagList</b></li>
  <li><b>CalendarScheduler</b></li>
  <li><b>Callback</b></li>
  <li><b>CandidateQueue</b></li>
  <li><b>CapabilityInformation</b></li>
  <li><b>Cara</b></li>
  <li><b>Channel</b></li>
  <li><b>ChannelList</b></li>
  <li><b>CoDelQueue</b></li>
  <li><b>CommandLine</b></li>
  <li><b>Config</b></li>
  <li><b>ConfigStore</b></li>
  <li><b>ConnectionManager</b></li>
  <li><b>ConstantRateWifiManager</b></li>
  <li><b>ConstantSpectrumPropagationLossModel</b></li>
  <li><b>ConstantVelocityHelper</b></li>
  <li><b>CosineAntennaModel</b></li>
  <li><b>Cost231PropagationLossModel</b></li>
  <li><b>CqaFfMacScheduler</b></li>
  <li><b>CsmaChannel</b></li>
  <li><b>CsmaHelper</b></li>
  <li><b>CsmaNetDevice</b></li>
  <li><b>CsmaStarHelper</b></li>
  <li><b>CtrlHeaders</b></li>
  <li><b>DataCalculator</b></li>
  <li><b>DataCollectionObject</b></li>
  <li><b>DataCollector</b></li>
  <li><b>DataOutputInterface</b></li>
  <li><b>DataRate</b></li>
  <li><b>DcaTxop</b></li>
  <li><b>Dcf</b></li>
  <li><b>DcfManager</b></li>
  <li><b>DefaultSimulatorImpl</b></li>
  <li><b>DeviceEnergyModel</b></li>
  <li><b>DeviceEnergyModelContainer</b></li>
  <li><b>DistributedSimulatorImpl</b></li>
  <li><b>Dot11sPeerManagementProtocol</b></li>
  <li><b>Double</b></li>
  <li><b>DoubleProbe</b></li>
  <li><b>DropTailQueue</b></li>
  <li><b>DsdvPacketQueue</b></li>
  <li><b>DsdvRoutingProtocol</b></li>
  <li><b>DsdvRoutingTable</b></li>
  <li><b>DsrErrorBuffer</b></li>
  <li><b>DsrFsHeader</b></li>
  <li><b>DsrGraReplyTable</b></li>
  <li><b>DsrHelper</b></li>
  <li><b>DsrMainHelper</b></li>
  <li><b>DsrMaintainBuffer</b></li>
  <li><b>DsrNetworkQueue</b></li>
  <li><b>DsrOptionHeader</b></li>
  <li><b>DsrOptions</b></li>
  <li><b>DsrRouting</b></li>
  <li><b>DsrSendBuffer</b></li>
  <li><b>DsssErrorRateModel</b></li>
  <li><b>EdcaTxopN</b></li>
  <li><b>EnergyHarvester</b></li>
  <li><b>EnergyHarvesterContainer</b></li>
  <li><b>EnergySource</b></li>
  <li><b>Enum</b></li>
  <li><b>EpcEnbApplication</b></li>
  <li><b>EpcHelper</b></li>
  <li><b>EpcMme</b></li>
  <li><b>EpcSgwPgwApplication</b></li>
  <li><b>EpcTft</b></li>
  <li><b>EpcTftClassifier</b></li>
  <li><b>EpcUeNas</b></li>
  <li><b>EpcX2</b></li>
  <li><b>EpcX2Header</b></li>
  <li><b>ErrorModel</b></li>
  <li><b>EthernetHeader</b></li>
  <li><b>EthernetTrailer</b></li>
  <li><b>EventId</b></li>
  <li><b>EventImpl</b></li>
  <li><b>FatalImpl</b></li>
  <li><b>FdBetFfMacScheduler</b></li>
  <li><b>FdMtFfMacScheduler</b></li>
  <li><b>FdNetDevice</b></li>
  <li><b>FdNetDeviceHelper</b></li>
  <li><b>FdReader</b></li>
  <li><b>FdTbfqFfMacScheduler</b></li>
  <li><b>FfMacScheduler</b></li>
  <li><b>FileAggregator</b></li>
  <li><b>FileHelper</b></li>
  <li><b>FlameProtocol</b></li>
  <li><b>FlameProtocolMac</b></li>
  <li><b>FlameRtable</b></li>
  <li><b>FlowIdTag</b></li>
  <li><b>FlowMonitor</b></li>
  <li><b>GlobalRouteManager</b></li>
  <li><b>GlobalRouteManagerImpl</b></li>
  <li><b>GlobalRouter</b></li>
  <li><b>GlobalRoutingHelper</b></li>
  <li><b>GlobalValue</b></li>
  <li><b>GnuplotAggregator</b></li>
  <li><b>GnuplotHelper</b></li>
  <li><b>GrantedTimeWindowMpiInterface</b></li>
  <li><b>GtkconfigStore</b></li>
  <li><b>GtpuHeader</b></li>
  <li><b>HalfDuplexIdealPhy</b></li>
  <li><b>HalfDuplexIdealPhySignalParameters</b></li>
  <li><b>Hash</b></li>
  <li><b>Hash-Fnv</b></li>
  <li><b>Hash-Murmur3</b></li>
  <li><b>HashFunction</b></li>
  <li><b>Header</b></li>
  <li><b>HeapScheduler</b></li>
  <li><b>HigherDataTxVectorTag</b></li>
  <li><b>Histogram</b></li>
  <li><b>HtCapabilities</b></li>
  <li><b>HwmpProtocol</b></li>
  <li><b>HwmpProtocolMac</b></li>
  <li><b>HwmpRtable</b></li>
  <li><b>HybridBuildingsPropagationLossModel</b></li>
  <li><b>Icmpv4Header</b></li>
  <li><b>Icmpv4L4Protocol</b></li>
  <li><b>Icmpv6Header</b></li>
  <li><b>Icmpv6L4Protocol</b></li>
  <li><b>Inet6SocketAddress</b></li>
  <li><b>InetSocketAddress</b></li>
  <li><b>InetTopologyReader</b></li>
  <li><b>Integer</b></li>
  <li><b>InterferenceHelper</b></li>
  <li><b>InternetStackHelper</b></li>
  <li><b>InternetTraceHelper</b></li>
  <li><b>IpL4Protocol</b></li>
  <li><b>IpcsClassifier</b></li>
  <li><b>IpcsClassifierRecord</b></li>
  <li><b>Ipv4</b></li>
  <li><b>Ipv4Address</b></li>
  <li><b>Ipv4AddressGenerator</b></li>
  <li><b>Ipv4AddressHelper</b></li>
  <li><b>Ipv4EndPoint</b></li>
  <li><b>Ipv4EndPointDemux</b></li>
  <li><b>Ipv4FlowProbe</b></li>
  <li><b>Ipv4GlobalRouting</b></li>
  <li><b>Ipv4Header</b></li>
  <li><b>Ipv4Interface</b></li>
  <li><b>Ipv4InterfaceAddress</b></li>
  <li><b>Ipv4L3Protocol</b></li>
  <li><b>Ipv4ListRouting</b></li>
  <li><b>Ipv4NixVectorRouting</b></li>
  <li><b>Ipv4PacketInfoTag</b></li>
  <li><b>Ipv4PacketProbe</b></li>
  <li><b>Ipv4RawSocketFactory</b></li>
  <li><b>Ipv4RawSocketFactoryImpl</b></li>
  <li><b>Ipv4RawSocketImpl</b></li>
  <li><b>Ipv4Route</b></li>
  <li><b>Ipv4RoutingProtocol</b></li>
  <li><b>Ipv4RoutingTableEntry</b></li>
  <li><b>Ipv4StaticRouting</b></li>
  <li><b>Ipv4StaticRoutingHelper</b></li>
  <li><b>Ipv6Address</b></li>
  <li><b>Ipv6AddressGenerator</b></li>
  <li><b>Ipv6AddressHelper</b></li>
  <li><b>Ipv6AutoconfiguredPrefix</b></li>
  <li><b>Ipv6EndPoint</b></li>
  <li><b>Ipv6EndPointDemux</b></li>
  <li><b>Ipv6Extension</b></li>
  <li><b>Ipv6ExtensionHeader</b></li>
  <li><b>Ipv6FlowProbe</b></li>
  <li><b>Ipv6Header</b></li>
  <li><b>Ipv6Interface</b></li>
  <li><b>Ipv6InterfaceAddress</b></li>
  <li><b>Ipv6L3Protocol</b></li>
  <li><b>Ipv6ListRouting</b></li>
  <li><b>Ipv6Option</b></li>
  <li><b>Ipv6OptionHeader</b></li>
  <li><b>Ipv6PacketProbe</b></li>
  <li><b>Ipv6PmtuCache</b></li>
  <li><b>Ipv6RawSocketImpl</b></li>
  <li><b>Ipv6StaticRouting</b></li>
  <li><b>Ipv6StaticRoutingHelper</b></li>
  <li><b>IsotropicAntennaModel</b></li>
  <li><b>ItuR1238PropagationLossModel</b></li>
  <li><b>ItuR1411LosPropagationLossModel</b></li>
  <li><b>ItuR1411NlosOverRooftopPropagationLossModel</b></li>
  <li><b>Jakes</b></li>
  <li><b>JakesProcess</b></li>
  <li><b>Kun2600MhzPropagationLossModel</b></li>
  <li><b>L2RateTracer</b></li>
  <li><b>LiIonEnergySource</b></li>
  <li><b>ListScheduler</b></li>
  <li><b>LlcSnalHeader</b></li>
  <li><b>LoopbackNetDevice</b></li>
  <li><b>LrWpanCsmaCa</b></li>
  <li><b>LrWpanErrorModel</b></li>
  <li><b>LrWpanHelper</b></li>
  <li><b>LrWpanInterferenceHelper</b></li>
  <li><b>LrWpanMac</b></li>
  <li><b>LrWpanNetDevice</b></li>
  <li><b>LrWpanPhy</b></li>
  <li><b>LrWpanSpectrumSignalParameters</b></li>
  <li><b>LrWpanSpectrumValueHelper</b></li>
  <li><b>LteAmc</b></li>
  <li><b>LteAnr</b></li>
  <li><b>LteChunkProcessor</b></li>
  <li><b>LteCommon</b></li>
  <li><b>LteControlMessage</b></li>
  <li><b>LteEnbMac</b></li>
  <li><b>LteEnbNetDevice</b></li>
  <li><b>LteEnbPhy</b></li>
  <li><b>LteEnbRrc</b></li>
  <li><b>LteFfrAlgorithm</b></li>
  <li><b>LteFfrDistributedAlgorithm</b></li>
  <li><b>LteFfrEnhancedAlgorithm</b></li>
  <li><b>LteFfrSoftAlgorithm</b></li>
  <li><b>LteFrHardAlgorithm</b></li>
  <li><b>LteFrNoOpAlgorithm</b></li>
  <li><b>LteFrSoftAlgorithm</b></li>
  <li><b>LteFrStrictAlgorithm</b></li>
  <li><b>LteGlobalPathlossDatabase</b></li>
  <li><b>LteHandoverAlgorithm</b></li>
  <li><b>LteHarqPhy</b></li>
  <li><b>LteHelper</b></li>
  <li><b>LteHexGridEnbTopologyHelper</b></li>
  <li><b>LteInterference</b></li>
  <li><b>LteMiErrorModel</b></li>
  <li><b>LteNetDevice</b></li>
  <li><b>LtePdcp</b></li>
  <li><b>LtePdcpHeader</b></li>
  <li><b>LtePhy</b></li>
  <li><b>LteRlc</b></li>
  <li><b>LteRlcAm</b></li>
  <li><b>LteRlcAmHeader</b></li>
  <li><b>LteRlcHeader</b></li>
  <li><b>LteRlcTm</b></li>
  <li><b>LteRlcUm</b></li>
  <li><b>LteRrcProtocolIdeal</b></li>
  <li><b>LteRrcProtocolReal</b></li>
  <li><b>LteSpectrumPhy</b></li>
  <li><b>LteSpectrumSignalParameters</b></li>
  <li><b>LteSpectrumValueHelper</b></li>
  <li><b>LteStatsCalculator</b></li>
  <li><b>LteUeMac</b></li>
  <li><b>LteUeNetDevice</b></li>
  <li><b>LteUePhy</b></li>
  <li><b>LteUePowerControl</b></li>
  <li><b>LteUeRrc</b></li>
  <li><b>LteVendorSpecificParameters</b></li>
  <li><b>MACMESSAGES</b></li>
  <li><b>Mac16Address</b></li>
  <li><b>Mac48Address</b></li>
  <li><b>Mac64Address</b></li>
  <li><b>MacLow</b></li>
  <li><b>MacRxMiddle</b></li>
  <li><b>MacStatsCalculator</b></li>
  <li><b>MakeEvent</b></li>
  <li><b>MapScheduler</b></li>
  <li><b>MeshL2RoutingProtocol</b></li>
  <li><b>MeshPointDevice</b></li>
  <li><b>MeshWifiInterfaceMac</b></li>
  <li><b>MicrowaveOvenSpectrumValue</b></li>
  <li><b>MinstrelWifiManager</b></li>
  <li><b>MobilityBuildingInfo</b></li>
  <li><b>MobilityHelper</b></li>
  <li><b>MpiInterface</b></li>
  <li><b>MsduAggregator</b></li>
  <li><b>MsduStandardAggregator</b></li>
  <li><b>MultiModelSpectrumChannel</b></li>
  <li><b>Names</b></li>
  <li><b>NdiscCache</b></li>
  <li><b>NetDevice</b></li>
  <li><b>NistErrorRateModel</b></li>
  <li><b>NixVector</b></li>
  <li><b>NoOpHandoverAlgorithm</b></li>
  <li><b>Node</b></li>
  <li><b>NodeList</b></li>
  <li><b>NonCommunicatingNetDevice</b></li>
  <li><b>Ns2MobilityHelper</b></li>
  <li><b>NullMessageMpiInterface</b></li>
  <li><b>NullMessageSimulatorImpl</b></li>
  <li><b>Object</b></li>
  <li><b>ObjectBase</b></li>
  <li><b>ObjectFactory</b></li>
  <li><b>ObjectPtrContainer</b></li>
  <li><b>OcbWifiMac</b></li>
  <li><b>OhBuildingsPropagationLossModel</b></li>
  <li><b>OkumuraHataPropagationLossModel</b></li>
  <li><b>OlsrHeader</b></li>
  <li><b>OlsrRoutingProtocol</b></li>
  <li><b>OmnetDataOutput</b></li>
  <li><b>OnOffApplication</b></li>
  <li><b>OnoeWifiRemoteStation</b></li>
  <li><b>OrbisTopologyReader</b></li>
  <li><b>OutputStreamWrapper</b></li>
  <li><b>Packet</b></li>
  <li><b>PacketBB</b></li>
  <li><b>PacketBurst</b></li>
  <li><b>PacketDataCalculators</b></li>
  <li><b>PacketLossCounter</b></li>
  <li><b>PacketMetadata</b></li>
  <li><b>PacketProbe</b></li>
  <li><b>PacketSink</b></li>
  <li><b>PacketSocket</b></li>
  <li><b>PacketSocketAddress</b></li>
  <li><b>PacketSocketClient</b></li>
  <li><b>PacketSocketFactory</b></li>
  <li><b>PacketSocketServer</b></li>
  <li><b>PacketTagList</b></li>
  <li><b>ParabolicAntennaModel</b></li>
  <li><b>PassiveBuffer</b></li>
  <li><b>PcapFile</b></li>
  <li><b>PcapFileWrapper</b></li>
  <li><b>PeerManagementProtocol</b></li>
  <li><b>PendingData</b></li>
  <li><b>PfFfMacScheduler</b></li>
  <li><b>PhyRxStatsCalculator</b></li>
  <li><b>PhyStatsCalculator</b></li>
  <li><b>PhyTxStatsCalculator</b></li>
  <li><b>Ping6Application</b></li>
  <li><b>PointToPointChannel</b></li>
  <li><b>PointToPointDumbbellHelper</b></li>
  <li><b>PointToPointEpcHelper</b></li>
  <li><b>PointToPointGridHelper</b></li>
  <li><b>PointToPointHelper</b></li>
  <li><b>PointToPointNetDevice</b></li>
  <li><b>PointToPointRemoteChannel</b></li>
  <li><b>PointToPointStarHelper</b></li>
  <li><b>Pointer</b></li>
  <li><b>PositionAllocator</b></li>
  <li><b>PppHeader</b></li>
  <li><b>PrintIntrospectedDoxygen</b></li>
  <li><b>Probe</b></li>
  <li><b>PropagationLossModel</b></li>
  <li><b>PssFfMacScheduler</b></li>
  <li><b>PyViz</b></li>
  <li><b>Queue</b></li>
  <li><b>RadioBearerStatsCalculator</b></li>
  <li><b>RadioBearerStatsConnector</b></li>
  <li><b>RadioEnvironmentMapHelper</b></li>
  <li><b>RadiotapHeader</b></li>
  <li><b>RadvdApplication</b></li>
  <li><b>RadvdHelper</b></li>
  <li><b>RadvdInterface</b></li>
  <li><b>RadvdPrefix</b></li>
  <li><b>RandomDirection2dMobilityModel</b></li>
  <li><b>RandomVariable</b></li>
  <li><b>RandomVariableStream</b></li>
  <li><b>RandomVariableStreamHelper</b></li>
  <li><b>RandomWalk2d</b></li>
  <li><b>RawTextConfig</b></li>
  <li><b>RedQueue</b></li>
  <li><b>RefCountBase</b></li>
  <li><b>RegularWifiMac</b></li>
  <li><b>RemSpectrumPhy</b></li>
  <li><b>RipNg</b></li>
  <li><b>RngSeedManager</b></li>
  <li><b>RngStream</b></li>
  <li><b>RocketfuelMapReader</b></li>
  <li><b>RocketfuelTopologyReader</b></li>
  <li><b>RocketfuelWeightsReader</b></li>
  <li><b>RouteCache</b></li>
  <li><b>RrFfMacScheduler</b></li>
  <li><b>RraaWifiManager</b></li>
  <li><b>RrcHeader</b></li>
  <li><b>RreqTable</b></li>
  <li><b>RttEstimator</b></li>
  <li><b>RvBatteryModel</b></li>
  <li><b>SNRToBlockErrorRateManager</b></li>
  <li><b>SSLinkManager</b></li>
  <li><b>SSManager</b></li>
  <li><b>SSScheduler</b></li>
  <li><b>Scheduler</b></li>
  <li><b>SeqTsHeader</b></li>
  <li><b>ServiceFlowManager</b></li>
  <li><b>ShannonSpectrumErrorModel</b></li>
  <li><b>SimpleChannel</b></li>
  <li><b>SimpleDeviceEnergyModel</b></li>
  <li><b>SimpleNetDevice</b></li>
  <li><b>SimpleNetDeviceHelper</b></li>
  <li><b>SimpleOfdmWimaxPhy</b></li>
  <li><b>Simulator</b></li>
  <li><b>SimulatorImpl</b></li>
  <li><b>SingleModelSpectrumChannel</b></li>
  <li><b>SixLowPanHelper</b></li>
  <li><b>SixLowPanNetDevice</b></li>
  <li><b>Socket</b></li>
  <li><b>SocketFactory</b></li>
  <li><b>SpectrumAnalyzer</b></li>
  <li><b>SpectrumAnalyzerHelper</b></li>
  <li><b>SpectrumConverter</b></li>
  <li><b>SpectrumInterference</b></li>
  <li><b>SpectrumModel</b></li>
  <li><b>SpectrumPhy</b></li>
  <li><b>SpectrumPropagationLossModel</b></li>
  <li><b>SpectrumSignalParameters</b></li>
  <li><b>SpectrumValue</b></li>
  <li><b>SqliteDataOutput</b></li>
  <li><b>SsServiceFlowManager</b></li>
  <li><b>StaWifiMac</b></li>
  <li><b>SubscriberStationNetDevice</b></li>
  <li><b>SupportedRates</b></li>
  <li><b>Synchronizer</b></li>
  <li><b>SystemCondition</b></li>
  <li><b>SystemMutex</b></li>
  <li><b>SystemPath</b></li>
  <li><b>SystemThread</b></li>
  <li><b>SystemWallClockMsPrivate</b></li>
  <li><b>TagBuffer</b></li>
  <li><b>TcpHeader</b></li>
  <li><b>TcpL4Protocol</b></li>
  <li><b>TcpNewReno</b></li>
  <li><b>TcpOption</b></li>
  <li><b>TcpOptionRfc793</b></li>
  <li><b>TcpOptionTS</b></li>
  <li><b>TcpOptionWinScale</b></li>
  <li><b>TcpReno</b></li>
  <li><b>TcpRfc793</b></li>
  <li><b>TcpRxBuffer</b></li>
  <li><b>TcpSocket</b></li>
  <li><b>TcpSocketBase</b></li>
  <li><b>TcpTahoe</b></li>
  <li><b>TcpTxBuffer</b></li>
  <li><b>TcpWestwood</b></li>
  <li><b>TdBetFfMacScheduler</b></li>
  <li><b>TdMtFfMacScheduler</b></li>
  <li><b>TdTbfqFfMacScheduler</b></li>
  <li><b>Test</b></li>
  <li><b>Time</b></li>
  <li><b>TimeDataCalculators</b></li>
  <li><b>TimeSeriesAdaptor</b></li>
  <li><b>Timer</b></li>
  <li><b>Tlv</b></li>
  <li><b>TopologyReader</b></li>
  <li><b>TopologyReaderHelper</b></li>
  <li><b>TraceFadingLossModel</b></li>
  <li><b>TraceHelper</b></li>
  <li><b>TraceSourceAccessor</b></li>
  <li><b>Trailer</b></li>
  <li><b>TtaFfMacScheduler</b></li>
  <li><b>TypeId</b></li>
  <li><b>UanChannel</b></li>
  <li><b>UanHelper</b></li>
  <li><b>UanMacAloha</b></li>
  <li><b>UanMacCw</b></li>
  <li><b>UanMacRc</b></li>
  <li><b>UanMacRcGw</b></li>
  <li><b>UanNetDevice</b></li>
  <li><b>UanPhyDual</b></li>
  <li><b>UanPhyGen</b></li>
  <li><b>UanPropModelThorp</b></li>
  <li><b>UanTransducerHd</b></li>
  <li><b>UanTxMode</b></li>
  <li><b>UdpClient</b></li>
  <li><b>UdpEchoClientApplication</b></li>
  <li><b>UdpEchoServerApplication</b></li>
  <li><b>UdpL4Protocol</b></li>
  <li><b>UdpServer</b></li>
  <li><b>UdpSocket</b></li>
  <li><b>UdpSocketImpl</b></li>
  <li><b>UdpTraceClient</b></li>
  <li><b>Uinteger</b></li>
  <li><b>Uinteger16Probe</b></li>
  <li><b>Uinteger32Probe</b></li>
  <li><b>Uinteger8Probe</b></li>
  <li><b>UplinkScheduler</b></li>
  <li><b>UplinkSchedulerMBQoS</b></li>
  <li><b>UplinkSchedulerRtps</b></li>
  <li><b>UplinkSchedulerSimple</b></li>
  <li><b>V4Ping</b></li>
  <li><b>Vector</b></li>
  <li><b>VendorSpecificAction</b></li>
  <li><b>VirtualNetDevice</b></li>
  <li><b>VisualSimulatorImpl</b></li>
  <li><b>Watchdog</b></li>
  <li><b>WaveMacLow</b></li>
  <li><b>WaveformGenerator</b></li>
  <li><b>WaveformGeneratorHelper</b></li>
  <li><b>WaypointMobilityModel</b></li>
  <li><b>WifiChannel</b></li>
  <li><b>WifiHelper</b></li>
  <li><b>WifiNetDevice</b></li>
  <li><b>WifiPhy</b></li>
  <li><b>WifiPhyStateHelper</b></li>
  <li><b>WifiRadioEnergyModel</b></li>
  <li><b>WifiRemoteStationManager</b></li>
  <li><b>WifiTxCurrentModel</b></li>
  <li><b>WimaxChannel</b></li>
  <li><b>WimaxHelper</b></li>
  <li><b>WimaxMacQueue</b></li>
  <li><b>WimaxNetDevice</b></li>
  <li><b>WimaxPhy</b></li>
  <li><b>XmlConfig</b></li>
  <li><b>YansErrorRateModel</b></li>
  <li><b>YansWifiChannel</b></li>
  <li><b>YansWifiHelper</b></li>
  <li><b>YansWifiPhy</b></li>
  <li><b>int64x64</b></li>
  <li><b>int64x64-128</b></li>
  <li><b>ndn.App</b></li>
  <li><b>ndn.AppDelayTracer</b></li>
  <li><b>ndn.AppFace</b></li>
  <li><b>ndn.AppHelper</b></li>
  <li><b>ndn.Consumer</b></li>
  <li><b>ndn.ConsumerBatches</b></li>
  <li><b>ndn.ConsumerCbr</b></li>
  <li><b>ndn.ConsumerWindow</b></li>
  <li><b>ndn.ConsumerZipfMandelbrot</b></li>
  <li><b>ndn.CsTracer</b></li>
  <li><b>ndn.FibHelper</b></li>
  <li><b>ndn.GlobalRoutingHelper</b></li>
  <li><b>ndn.L3Protocol</b></li>
  <li><b>ndn.L3RateTracer</b></li>
  <li><b>ndn.LinkControlHelper</b></li>
  <li><b>ndn.NetDeviceFace</b></li>
  <li><b>ndn.Producer</b></li>
  <li><b>ndn.RttEstimator</b></li>
  <li><b>ndn.RttMeanDeviation</b></li>
  <li><b>ndn.StackHelper</b></li>
  <li><b>ndn.StrategyChoiceHelper</b></li>
  <li><b>ndn.cs.ContentStore</b></li>
  <li><b>ndn.cs.Fifo</b></li>
  <li><b>ndn.cs.Fifo::AggregateStats</b></li>
  <li><b>ndn.cs.Fifo::Freshness</b></li>
  <li><b>ndn.cs.Fifo::LifetimeStats</b></li>
  <li><b>ndn.cs.Freshness.Fifo</b></li>
  <li><b>ndn.cs.Freshness.Lfu</b></li>
  <li><b>ndn.cs.Freshness.Lru</b></li>
  <li><b>ndn.cs.Freshness.Random</b></li>
  <li><b>ndn.cs.Lfu</b></li>
  <li><b>ndn.cs.Lfu::AggregateStats</b></li>
  <li><b>ndn.cs.Lfu::Freshness</b></li>
  <li><b>ndn.cs.Lfu::LifetimeStats</b></li>
  <li><b>ndn.cs.Lru</b></li>
  <li><b>ndn.cs.Lru::AggregateStats</b></li>
  <li><b>ndn.cs.Lru::Freshness</b></li>
  <li><b>ndn.cs.Lru::LifetimeStats</b></li>
  <li><b>ndn.cs.Nocache</b></li>
  <li><b>ndn.cs.ProbabilityImpl::Fifo</b></li>
  <li><b>ndn.cs.ProbabilityImpl::Lfu</b></li>
  <li><b>ndn.cs.ProbabilityImpl::Lru</b></li>
  <li><b>ndn.cs.ProbabilityImpl::Random</b></li>
  <li><b>ndn.cs.Random</b></li>
  <li><b>ndn.cs.Random::AggregateStats</b></li>
  <li><b>ndn.cs.Random::Freshness</b></li>
  <li><b>ndn.cs.Random::LifetimeStats</b></li>
  <li><b>ndn.cs.Stats.Fifo</b></li>
  <li><b>ndn.cs.Stats.Lfu</b></li>
  <li><b>ndn.cs.Stats.Lru</b></li>
  <li><b>ndn.cs.Stats.Random</b></li>
  <li><b>nfd.BestRouteStrategy2</b></li>
  <li><b>nfd.ChannelStatusPublisher</b></li>
  <li><b>nfd.ClientControlStrategy</b></li>
  <li><b>nfd.CommandValidator</b></li>
  <li><b>nfd.ConfigFile</b></li>
  <li><b>nfd.ContentStore</b></li>
  <li><b>nfd.CsEntry</b></li>
  <li><b>nfd.CsSkipListEntry</b></li>
  <li><b>nfd.DeadNonceList</b></li>
  <li><b>nfd.FaceManager</b></li>
  <li><b>nfd.FaceQueryStatusPublisher</b></li>
  <li><b>nfd.FaceStatusPublisher</b></li>
  <li><b>nfd.FaceTable</b></li>
  <li><b>nfd.FibEnumerationPublisher</b></li>
  <li><b>nfd.FibManager</b></li>
  <li><b>nfd.Forwarder</b></li>
  <li><b>nfd.GeneralConfigSection</b></li>
  <li><b>nfd.InternalFace</b></li>
  <li><b>nfd.ManagerBase</b></li>
  <li><b>nfd.NameTree</b></li>
  <li><b>nfd.PrivilegeHelper</b></li>
  <li><b>nfd.Strategy</b></li>
  <li><b>nfd.StrategyChoice</b></li>
  <li><b>nfd.StrategyChoiceManager</b></li>
  <li><b>nfd.StrategyChoicePublisher</b></li>
  <li><b>nfd.TablesConfigSection</b></li>
  <li><b>ns3::ArfWifiManager</b></li>
  <li><b>simpleOfdmWimaxChannel</b></li>
</ul>
*/

