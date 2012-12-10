/*
 * c2s_handler.cpp
 *
 *  Created on: 2012-11-5
 *      Author: root
 */

#include "c2s_handler.h"
#include "rpc_handler.h"
//#include "relay_target_manager.h"

using namespace service_engine;

namespace c2s
{

C2SHandler::C2SHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl,RelayLine& line,boost::shared_ptr<RelayStubsScheduler> scheduler)
: base::packet::PacketHandler(fd, addr, reactor_impl),
	conn_id_(0),
	line_(line),
	scheduler_(scheduler)
{
}

C2SHandler::~C2SHandler()
{
}

void C2SHandler::onOpen()
{
	LOG(info) << "c2s handler open: " << addr() << ENDL;

}

void C2SHandler::onPacketArrive(const base::packet::Header& header, base::packet::Packet& body)
{
	LOG(trace) << "c2s packet arrive: " << header << ENDL;

	try
	{
		PacketInputStream packetinput(body);
		::google::protobuf::io::CodedInputStream in(&packetinput);

		auto_ptr<rpc::MessageBody> mb(new rpc::MessageBody);
		if (!mb->ParseFromCodedStream(&in))
			THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_PARSE_ERROR, "parse error!");

		if (scheduler_)
		{
			scheduler_->invokeStub(boost::static_pointer_cast<C2SHandler>(this->shared_from_this()), mb);
		}

	}
	catch (exception& e)
	{
		LOG(error) << EXCEPTION_DIAG_INFO(e);
	}
}

void C2SHandler::onDisconnected()
{
	LOG(info) << "c2s handler disconnected: " << addr() << ENDL;
}

void C2SHandler::relay(service_engine::rpc::MessageBody& body)
{
	try
	{
		base::packet::Packet out;
		body.SerializeWithCachedSizesToArray((uint8_t*)out.extendTailConinuous(body.ByteSize()));

		write(base::packet::Header(out.getSize()), out);
	}
	catch (::google::protobuf::FatalException& e)
	{
		THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_INTERNAL_ERROR, e.what());
	}
	catch (base::net::NetException& e)
	{
		THROW_RPC_CALL_EXCEPTION(rpc::EC_RPC_TRANSPORT_ERROR, e.what());
	}
}

void C2SHandler::sendFailedResponse(const rpc::MessageBody& request_body, int err_code, const char* err_msg)
{
	try
	{
		string data;
		rpc::FailedResponse failedresp;
		failedresp.set_err_code(err_code);
		failedresp.set_err_msg(err_msg);
		failedresp.SerializeToString(&data);

		rpc::MessageBody mb;
		mb.set_op(rpc::OP_RESPONSE_FAILED);
		mb.set_cmd(request_body.cmd());
		mb.set_seq(request_body.seq());
		for (int i=0; i<request_body.router_size(); ++i)
			mb.add_router(request_body.router(i));
		mb.set_data(data);

		base::packet::Packet out;
		mb.SerializeWithCachedSizesToArray((uint8_t*)out.extendTailConinuous(mb.ByteSize()));
		write(base::packet::Header(out.getSize()), out);
	}
	catch (exception& e)
	{
		LOG(error) << "send response failed!" << EXCEPTION_DIAG_INFO(e) << ENDL;
	}
}




} /* namespace c2s */
