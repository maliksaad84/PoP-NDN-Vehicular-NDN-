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

#include "ndn-consumerO.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

#include "utils/ndn-ns3-packet-tag.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <ndn-cxx/lp/tags.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

///////////////////////////////////////

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>


#include <fstream>

using namespace std;
int array0071[1000][10];

///////////////////////////////////////

NS_LOG_COMPONENT_DEFINE("ndn.MyChange");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(MyChange);

////////////////////////////////////////
class recommendationCalculation {

private:
uint32_t contentArray[1000][10];
double contentRatingMatrix[1000][10];

public:
uint32_t contentListSize1    =   1000;
uint32_t contentListSize     =   10;

void contentReadFunction();
void contentDisplayFunction();
void contentRecordFunction(uint32_t seq, int64_t time1);
int32_t contentCalculationFunction(uint32_t seq, int32_t array[], int64_t time1);
void contentWriteFunction();


};

void recommendationCalculation::contentReadFunction(){
//    ifstream fileRead;
//    fileRead.open("contentMatt.txt");

    for (uint32_t n1=0; n1<contentListSize1; n1++)
    {
            for (uint32_t n2=0;  n2<contentListSize; n2++)
            {
//                fileRead >> contentArray[n1][n2];
            	contentArray[n1][n2] = array0071[n1][n2];
            }
    }
//    fileRead.close();
}

void recommendationCalculation::contentDisplayFunction(){

    for (uint32_t n11=0; n11<contentListSize1 ;n11++)
    {
            for (uint32_t n12=0; n12<contentListSize ;n12++)
            {
                cout<<" "<< contentArray[n11][n12];
            }
            cout<<endl;
    }
//    cout<<" contentDisplayFunction() File Reading is complete with the numbers loaded in the variables"<<endl;
}

void recommendationCalculation::contentRecordFunction(uint32_t seq, int64_t time1){

	int64_t interval =  100;// one Event(row) comprises of 100 seconds
	int64_t timecalc =  1;
    uint32_t row =   0;
    uint32_t tenTimes    =   0;
    uint32_t localSeq =  0;

    while ((timecalc*interval)<=time1) // finding the row of the vector whose value is to be updated!
    {
        row++;
        timecalc++;
        // cout<<"\nI am in ."<<endl;
    }

    if(seq!=0) // finding the sequence number associated to the sequence received!
    {
        for (uint32_t v1=1; v1<=seq; v1++)
        {
            if(remainder(v1,10)==0)
            {
                tenTimes++;
            }
        }
    }

    localSeq = seq - tenTimes*10;
    contentArray[row][localSeq] = contentArray[row][localSeq]+1;
//    cout<<"\nI am out "<< seq << " seq " << localSeq << " localSeq " << contentArray[row][localSeq] << " contentArray[row][localSeq]" << endl;
//    cout<<endl;
}

int32_t recommendationCalculation::contentCalculationFunction(uint32_t seq, int32_t array[], int64_t time1){
//int contentRatingMatrix[10][10];

    double contentRatingMatrixLocal[10][10];
//    double contentRatingMatrixOneRow1[1][10];

    for (uint32_t n11=0; n11<contentListSize ;n11++)
    {
            for (uint32_t n12=0; n12<contentListSize ;n12++)
            {
                contentRatingMatrixLocal[n11][n12] = 0;
            }
    }

    for (uint32_t n11=0; n11<contentListSize; n11++)
    {
    	uint32_t intrm_vari[contentListSize] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            for (uint32_t n12=0; n12<contentListSize; n12++)
            {
                if(n11!=n12){
                    for (uint32_t n13=0; n13<contentListSize1; n13++)
                    {
                        intrm_vari[n12] = intrm_vari[n12] + (contentArray[n13][n11]*contentArray[n13][n12]);
                    //    cout << n11 <<" "<<n12<<" "<<n13<<endl;
                    //    cout << intrm_vari[n12] <<"="<<contentArray[n13][n11]<<"*"<<contentArray[n13][n12]<<endl;
                    }
               // cout<< intrm_vari[n12] <<endl;
                }
            contentRatingMatrixLocal[n12][n11] = intrm_vari[n12];
            }
//                cout<<endl;
//                cout<< intrm_vari[n11] << " ";
//        cout<<endl;
    }
//            cout<<endl;
//    cout<<"----------------------------"<<endl;
    for (uint32_t n11=0; n11<contentListSize ;n11++)
    {
            for (uint32_t n12=0; n12<contentListSize ;n12++)
            {
//               cout << contentRatingMatrixLocal[n11][n12] << " ";
            }
//            cout<<endl;
    }
//    cout<<"----------------------------"<<endl;

//    cout<<"----------------------------"<<endl;
//   for (int n11=0; n11<contentListSize; n11++)
//    {
            for (int n12=0; n12<contentListSize; n12++)
            {
                double localsum =0;
                for (int n13=0; n13<contentListSize; n13++)
                {
                    localsum = localsum + contentRatingMatrixLocal[n13][n12];
                }

                for (int aj = 0; aj<contentListSize; aj++)
                {
                   contentRatingMatrix [aj][n12] = (contentRatingMatrixLocal[aj][n12])/localsum;
                }
            }
//    cout<<"----------------------------"<<endl;
    for (int n11=0; n11<contentListSize ;n11++)
    {
            for (int n12=0; n12<contentListSize ;n12++)
            {
//               cout << contentRatingMatrix[n11][n12] << " ";
            }
//            cout<<endl;
    }
//    cout<<"----------------------------"<<endl;

    int interval =  100;
    int timecalc =  1;
    uint32_t row =   0;
    uint32_t tenTimes    =   0;
    uint32_t tenTimes1    =   0;

    uint32_t localSeq =  0;
    uint32_t localSeq1 =  0;

    double contentRatingMatrixValues[10];
    double contentRatingMatrixValues1[10];

    while ((timecalc*interval)<=time1) // finding the row of the vector whose value is to be updated!
    {
        row++;
        timecalc++;
        //cout<<"\nI am in ."<<endl;
    }

    if(seq!=0) // finding the sequence number associated to the sequence received!
    {
        for (uint32_t v1=1; v1<=seq; v1++)
        {
            if(remainder(v1,10)==0)
            {
                tenTimes++;
            }
        }
    }

    localSeq1 = seq - tenTimes*10;
    tenTimes1	=	tenTimes;
//    cout<<  localSeq1 <<" = "<< seq <<" - "<< tenTimes <<" *10"<<endl;

    for(uint32_t iter01=0; iter01<contentListSize; iter01++) //copying the array elements from content similarity matrix
    {
        contentRatingMatrixValues[iter01] = contentRatingMatrix[iter01][localSeq1];
        contentRatingMatrixValues1[iter01] = contentRatingMatrix[iter01][localSeq1];

   //     cout << contentRatingMatrixValues[iter01] << " ";
    }
 //   cout<<endl;

    uint32_t itt = 0;
    uint32_t itt1 =-1;
    uint32_t itt2;
   do{
        for(uint32_t iter01=0; iter01<contentListSize; iter01++) //copying the array elements from content similarity matrix
        {
            if(itt!=iter01)
            {
                if(contentRatingMatrixValues[iter01] > contentRatingMatrixValues[itt])
                {
                    itt = iter01;
                }
            }
        }
        if(itt1==-1)
        {
            itt2 = itt;
        }
        contentRatingMatrixValues[itt] = 0;
        itt1++;
    }
    while(array[(itt+(tenTimes*10))]==(itt+(tenTimes*10)) && itt1 != 9);

 //   cout << contentRatingMatrixValues[itt] << " " << itt << " <'-'.'-'.'-'.'-'>" <<endl;
//    cout << " array[(itt+(tenTimes*10))] "  << array[(itt+(tenTimes*10))] << " " << seq << " " << localSeq1 << " " << itt2 << endl;
//    cout << " (array[(itt+(tenTimes*10))]==(itt+(tenTimes*10)) && itt1 != 9 "  << (array[(itt+(tenTimes*10))]==(itt+(tenTimes*10)) && itt1 != 9) << endl;
    if (seq <10)
    {
        itt=0;
		tenTimes =0;
      //  cout << " ---- 00 ---- " << endl;

    }

    else if (seq >= 10)
    {
        if (itt<localSeq1)
        {
        	if( contentRatingMatrixValues1[itt+(tenTimes*10)]< 0.111){

        //		cout << itt << " " << contentRatingMatrixValues1[itt+(tenTimes*10)] <<" ---- 01 ---- " << endl;
        		itt = 0;
        		tenTimes =0;

        	}
        	else{
        		itt = itt + 10 ;
         //       cout << " ---- 02 ---- " << endl;

        	}
        }
        if (itt1==9)
        {
        	if( contentRatingMatrixValues1[itt2+(tenTimes*10)]< 0.111){
        		itt = 0;
        		tenTimes =0;
          //      cout << " ---- 11 ---- " << endl;
        	}
        	else{
         //       cout << " ---- 12 ---- " << endl;
            itt = itt2 + 10;
            tenTimes	= tenTimes1;
        	}
        }
       // cout << " array[(itt+(tenTimes*10))] "  << array[(itt+(tenTimes*10))] << endl;
    }
//    cout << " itt "  << itt << endl;
//    cout << " array[(itt+(tenTimes*10))] "  << itt+(tenTimes*10) << endl;

    return(itt+(tenTimes*10));
}


void recommendationCalculation::contentWriteFunction(){

//    ofstream fileWrite;
//    fileWrite.open("contentMatt.txt");

    for (uint32_t n21=0; n21<contentListSize1; n21++)
    {
            for (uint32_t n22=0; n22<contentListSize; n22++)
            {
            	array0071[n21][n22] = contentArray[n21][n22];
//                fileWrite << contentArray[n21][n22];
//                if (n22<(contentListSize-1))
//                {
//                    fileWrite << " ";
//                }
            }
//            fileWrite << "\n";
    }

//    fileWrite.close();
//    cout<<"File has been re-editted\n "<<endl;
}


////////////////////////////////////////

TypeId
MyChange::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::MyChange")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddAttribute("StartSeq", "Initial sequence number", IntegerValue(0),
                    MakeIntegerAccessor(&MyChange::m_seq), MakeIntegerChecker<int32_t>())

     // .AddAttribute("Prefix", "Name of the Interest", StringValue("/prefix"),
                   // MakeNameAccessor(&MyChange::m_interestName), MakeNameChecker())
    //  .AddAttribute("Prefix", "Name of the Interest", StringValue("/Malik"),
					//.AddAttribute(MakeNameAccessor(&MyChange::m_interestNamestar), MakeNameChecker())
      .AddAttribute("LifeTime", "LifeTime for interest packet", StringValue("1000ms"),
                    MakeTimeAccessor(&MyChange::m_interestLifeTime), MakeTimeChecker())

      .AddAttribute("RetxTimer",
                    "Timeout defining how frequent retransmission timeouts should be checked",
                    StringValue("1000ms"),
                    MakeTimeAccessor(&MyChange::GetRetxTimer, &MyChange::SetRetxTimer),
                    MakeTimeChecker())

      .AddTraceSource("LastRetransmittedInterestDataDelay",
                      "Delay between last retransmitted Interest and received Data",
                      MakeTraceSourceAccessor(&MyChange::m_lastRetransmittedInterestDataDelay),
                      "ns3::ndn::MyChange::LastRetransmittedInterestDataDelayCallback")

      .AddTraceSource("FirstInterestDataDelay",
                      "Delay between first transmitted Interest and received Data",
                      MakeTraceSourceAccessor(&MyChange::m_firstInterestDataDelay),
                      "ns3::ndn::MyChange::FirstInterestDataDelayCallback")
  .AddAttribute(
  					         "Postfix",
  					         "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
  					         StringValue("/"), MakeNameAccessor(&MyChange::m_postfix), MakeNameChecker())
  					 //     .AddAttribute("PayloadSize", "Virtual payload size for Content packets", UintegerValue(1024),
  					    //                MakeUintegerAccessor(&MyChange::m_virtualPayloadSize),
  					   //                 MakeUintegerChecker<uint32_t>())
  					      .AddAttribute("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
  					                    TimeValue(Seconds(0)), MakeTimeAccessor(&MyChange::m_freshness),
  					                    MakeTimeChecker())
  					      .AddAttribute(
  					         "Signature",
  					         "Fake signature, 0 valid signature (default), other values application-specific",
  					         UintegerValue(0), MakeUintegerAccessor(&MyChange::m_signature),
  					         MakeUintegerChecker<uint32_t>())
  					      .AddAttribute("KeyLocator",
  					                    "Name to be used for key locator.  If root, then key locator is not used",
  					                    NameValue(), MakeNameAccessor(&MyChange::m_keyLocator), MakeNameChecker())

      ;

  return tid;
}

MyChange::MyChange()
  : m_rand(CreateObject<UniformRandomVariable>())
  , m_seq(0)
  , m_seqMax(0) // don't request anything
{
  NS_LOG_FUNCTION_NOARGS();

  m_rtt = CreateObject<RttMeanDeviation>();
}

void
MyChange::SetRetxTimer(Time retxTimer)
{
  m_retxTimer = retxTimer;
  if (m_retxEvent.IsRunning()) {
    // m_retxEvent.Cancel (); // cancel any scheduled cleanup events
    Simulator::Remove(m_retxEvent); // slower, but better for memory
  }

  // schedule even with new timeout
  m_retxEvent = Simulator::Schedule(m_retxTimer, &MyChange::CheckRetxTimeout, this);
}

Time
MyChange::GetRetxTimer() const
{
  return m_retxTimer;
}

void
MyChange::CheckRetxTimeout()
{
  Time now = Simulator::Now();

  Time rto = m_rtt->RetransmitTimeout();
  // NS_LOG_DEBUG ("Current RTO: " << rto.ToDouble (Time::S) << "s");

  while (!m_seqTimeouts.empty()) {
    SeqTimeoutsContainer::index<i_timestamp>::type::iterator entry =
      m_seqTimeouts.get<i_timestamp>().begin();
    if (entry->time + rto <= now) // timeout expired?
    {
      uint32_t seqNo = entry->seq;
      m_seqTimeouts.get<i_timestamp>().erase(entry);
      OnTimeout(seqNo);
    }
    else
      break; // nothing else to do. All later packets need not be retransmitted
  }

  m_retxEvent = Simulator::Schedule(m_retxTimer, &MyChange::CheckRetxTimeout, this);
}

// Application Methods
void
MyChange::StartApplication() // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS();

  // do base stuff
  App::StartApplication();

 //Simulator::Schedule(Seconds(0.6), &MyChange::SendPacket, this);
 // Simulator::Schedule(Seconds(1.0), &MyChange::SendPacketstar, this);

 // ScheduleNextPacket();
}

void
MyChange::StopApplication() // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS();

  // cancel periodic packet generation
  Simulator::Cancel(m_sendEvent);

  // cleanup base stuff
  App::StopApplication();
}

void
MyChange::SendPacket()
{
  if (!m_active)
    return;

if(ss!=0){
  if (dataReceivedSeqNumber[ss]==ss)
  {
	  NS_LOG_INFO("Tester1 " << dataReceivedSeqNumber[ss]<< " "<<ss);

	  return;
  }
}
	  /*
  uint32_t seq = std::numeric_limits<uint32_t>::max(); // invalid

  while (m_retxSeqs.size()) {
    seq = *m_retxSeqs.begin();
    m_retxSeqs.erase(m_retxSeqs.begin());
    break;
  }

  if (seq == std::numeric_limits<uint32_t>::max()) {
    if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
      if (m_seq >= m_seqMax) {
        return; // we are totally done
      }
    }

    seq = m_seq++;
  }
  */
  NS_LOG_INFO("Tester2 ");

  m_interestName="/Monet1";
  //
  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
  nameWithSequence->appendSequenceNumber(ss);
  //

  // shared_ptr<Interest> interest = make_shared<Interest> ();
  shared_ptr<Interest> interest = make_shared<Interest>();
  interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setName(*nameWithSequence);
  interest->setCanBePrefix(false);
  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
  interest->setInterestLifetime(interestLifeTime);

  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
  NS_LOG_INFO("> Interest for " << ss);

  WillSendOutInterest(ss);
  NS_LOG_INFO("Tester3 ");

  m_transmittedInterests(interest, this, m_face);
  m_appLink->onReceiveInterest(*interest);

  //ScheduleNextPacket();

  //Simulator::Schedule(Seconds(0.6), &MyChange::SendPacket, this);
}


void
MyChange::SendPacketstar()
{
	  if (!m_active)
	    return;

	  NS_LOG_FUNCTION_NOARGS();

	  m_interestName="/Monet1";
	  //
	  shared_ptr<Name> nameWithSequence = make_shared<Name>(m_interestName);
	  nameWithSequence->appendSequenceNumber(newSeq);

	  dataRecommendeddSeqNumber[newSeq] = newSeq;
	  recommendedContentCounter = recommendedContentCounter + 1; /////////////////////////////////////////////To be output
//cout<<"recommendedContentProvider*****   recommendedContentProvider   "<<recommendedContentCounter;

NS_LOG_INFO( " @	r@	@2rrrrrrrrrrrr	@	@	@	@	@		  1-2-3 " << "******" << "********** ----- " << recommendedContentCounter);
	  //
	  // shared_ptr<Interest> interest = make_shared<Interest> ();
	  shared_ptr<Interest> interest = make_shared<Interest>();
	  interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
	  interest->setName(*nameWithSequence);
	  interest->setCanBePrefix(false);
	  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
	  interest->setInterestLifetime(interestLifeTime);

	  // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
	  NS_LOG_INFO("> Interest for " << newSeq);

	  WillSendOutInterest(newSeq);
///////////////////////////////////////////////////////////////////////////////////new value //////////////////////////////////////

	  fstream fout;

	          // opens an existing csv file or creates a new file.
	          fout.open("recommended.csv", ios::out | ios::app);

	          fout << recommendedContentCounter << ", "
	          //<< stim << ", "
	          << "\n";


	  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	  m_transmittedInterests(interest, this, m_face);
	  m_appLink->onReceiveInterest(*interest);
}



///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
MyChange::OnData(shared_ptr<const Data> data)
{
  if (!m_active)
    return;

  App::OnData(data); // tracing inside

  NS_LOG_FUNCTION(this << data);

  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

  // This could be a problem......
  uint32_t seq = data->getName().at(-1).toSequenceNumber();
  NS_LOG_INFO("<  DATA for " << seq  <<" "<<data->getName());
  dataReceivedSeqNumber[seq]=seq;

  int hopCount = 0;
  auto hopCountTag = data->getTag<lp::HopCountTag>();
  if (hopCountTag != nullptr) { // e.g., packet came from local node's cache
    hopCount = *hopCountTag;
  }
 NS_LOG_DEBUG("Hop count: " << hopCount);

  SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find(seq);
  if (entry != m_seqLastDelay.end()) {
    m_lastRetransmittedInterestDataDelay(this, seq, Simulator::Now() - entry->time, hopCount);
  }

  entry = m_seqFullDelay.find(seq);
  if (entry != m_seqFullDelay.end()) {
    m_firstInterestDataDelay(this, seq, Simulator::Now() - entry->time, m_seqRetxCounts[seq], hopCount);
  }

  m_seqRetxCounts.erase(seq);
  m_seqFullDelay.erase(seq);
  m_seqLastDelay.erase(seq);

  m_seqTimeouts.erase(seq);
  m_retxSeqs.erase(seq);

  m_rtt->AckSeq(SequenceNumber32(seq));
  /////////////////////////////////////Changed////////////////////////////////////////////

  m_dataName = "/Monet";
  shared_ptr<Name> nameWithSequencestar = make_shared<Name>(m_dataName);
  nameWithSequencestar->appendSequenceNumber(seq);

  if (interestReceivedSeqNumber[seq]==seq)
  {
    auto data1 = make_shared<Data>();
    data1->setName(*nameWithSequencestar);
    data1->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));
    data1->setContent(make_shared< ::ndn::Buffer>(1024/3));
    data1->setTag(make_shared<lp::HopCountTag>(hopCount));

             int hopCount1 = 0;
             auto hopCountTag1 = data1->getTag<lp::HopCountTag>();

             if (hopCountTag1 != nullptr) { // e.g., packet came from local node's cache
                 hopCount1 = *hopCountTag1;
               }


             NS_LOG_UNCOND("Hop count***********123123*************: " << *hopCountTag1);
    Signature signature;
    SignatureInfo signatureInfo(static_cast< ::ndn::tlv::SignatureTypeValue>(255));

    if (m_keyLocator.size() > 0) {
      signatureInfo.setKeyLocator(m_keyLocator);
    }

    signature.setInfo(signatureInfo);
    signature.setValue(::ndn::makeNonNegativeIntegerBlock(::ndn::tlv::SignatureValue, m_signature));

    data1->setSignature(signature);

    //NS_LOG_INFO("node(" << GetNode()->GetId() << ") responding with Data: " << data1->getName());

    // to create real wire encoding
    data1->wireEncode();

    NS_LOG_INFO("< DATA1 for " << seq  <<" "<<data1->getName());

    m_transmittedDatas(data1, this, m_face);
    m_appLink->onReceiveData(*data1);
  }

  /////////////////////////////////
  Time now = Simulator::Now();
//  uint32_t seq = interest->getName().at(-1).toSequenceNumber();
  int64_t time6 = now.ToInteger(Time::S);
  NS_LOG_INFO("In Producer " << time6);

  recommendationCalculation Rc;

//  dataReceivedSeqNumber[seq]=seq;

  Rc.contentReadFunction();
 // Rc.contentDisplayFunction();
//  Rc.contentRecordFunction(seq,time6);

  newSeq = Rc.contentCalculationFunction(seq, dataReceivedSeqNumber ,time6);
 // Rc.contentWriteFunction();

  NS_LOG_INFO("In Intermediate Node " << seq << " "<< newSeq);

  /////////////////////////////////

  if(newSeq>0)
    {
    //newSeq = seq+1;
	  SendPacketstar();
    }

  ////////////////////////////////////////////Till this/////////////////////////////
  }

/*
void
MyChange::OnNack(shared_ptr<const lp::Nack> nack)
{
  /// tracing inside
  App::OnNack(nack);

  NS_LOG_INFO("NACK received for: " << nack->getInterest().getName()
              << ", reason: " << nack->getReason());
}
*/
void
MyChange::OnTimeout(uint32_t sequenceNumber)
{
  NS_LOG_FUNCTION(sequenceNumber);
  // std::cout << Simulator::Now () << ", TO: " << sequenceNumber << ", current RTO: " <<
  // m_rtt->RetransmitTimeout ().ToDouble (Time::S) << "s\n";

  m_rtt->IncreaseMultiplier(); // Double the next RTO
  m_rtt->SentSeq(SequenceNumber32(sequenceNumber),
                 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert(sequenceNumber);
  ScheduleNextPacket();
}
void
MyChange::WillSendOutInterest(uint32_t sequenceNumber)
{
  NS_LOG_DEBUG("Trying to add " << sequenceNumber << " with " << Simulator::Now() << ". already "
                                << m_seqTimeouts.size() << " items");

  m_seqTimeouts.insert(SeqTimeout(sequenceNumber, Simulator::Now()));
  m_seqFullDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqLastDelay.erase(sequenceNumber);
  m_seqLastDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqRetxCounts[sequenceNumber]++;

  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1);
}

void
MyChange::WillSendOutIntereststar(uint32_t sequenceNumber)
{
  NS_LOG_DEBUG("Trying to add " << sequenceNumber << " with " << Simulator::Now() << ". already "
                                << m_seqTimeouts.size() << " items");

  m_seqTimeouts.insert(SeqTimeout(sequenceNumber, Simulator::Now()));
  m_seqFullDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqLastDelay.erase(sequenceNumber);
  m_seqLastDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqRetxCounts[sequenceNumber]++;

  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1);
}


} // namespace ndn
} // namespace ns3
