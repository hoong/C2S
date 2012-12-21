#include "relay_handler.h"


namespace relay
{


virtual void RelayHandler::onPacketArrive(
		const base::packet::Header& header, 
		base::packet::Packet& body)
{
	LOG(trace) << "packet arrive: " << header << ENDL;

	try
	{
		rpc::Body msgbody;
		PacketInputStream packetinput(body);
		::google::protobuf::io::CodedInputStream in(&packetinput);

		if (!msgbody.ParseFromCodedStream(&in))
		{
			LOG(error) << "parse packet error! " << header << ENDL;
			return;
		}
		rpc::AddrList relay_addrs,local_addrs;
		for (rpc::AddrList::iterator it = msgbody.targets().begin();
				it != msgbody.targets().end(); 
				++it)
		{
			if (isLocal(*it))
			{
				relay_addrs.add(*it);
			}
			else
			{
				local_addrs.add(*it);
			}
		}

		if (relay_addrs.size() >1)
		{
			onRelay(msgbody.from(), relay_addrs, *msgbody.mutable_msgs());
		}

		if (local_addrs.size() >1)
		{
			onMessage(msgbody.from(), local_addrs, *msgbody.mutable_msgs());
		}
	}
	catch (exception& e)
	{
		LOG(error) << EXCEPTION_DIAG_INFO(e);
	}


}
