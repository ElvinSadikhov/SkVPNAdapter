// Private Gateway
// Copyright (C) 2012-2020 SkVPN Technologies, Inc.
// All rights reserved

#pragma once

//#define SKVPN_PROCFS_DEBUG

#include <string>

#include <skvpn/common/string.hpp>
#include <skvpn/common/exception.hpp>
#include <skvpn/common/fileunix.hpp>
#include <skvpn/common/sleep.hpp>
#include <skvpn/common/stat.hpp>
#include <skvpn/common/format.hpp>
#include <skvpn/common/action.hpp>
#include <skvpn/common/stop.hpp>
#include <skvpn/buffer/bufstr.hpp>

namespace skvpn {

  class ProcFS : public Action
  {
  public:
    SKVPN_EXCEPTION(procfs_error);

    ProcFS(std::string fn_arg, std::string text_arg)
      : fn(std::move(fn_arg)),
	text(std::move(text_arg))
    {
    }

    virtual void execute(std::ostream& os) override
    {
      os << to_string() << std::endl;
      try {
	write_sys(fn, text);
      }
      catch (const std::exception& e)
	{
	  os << "ProcFS exception: " << e.what() << std::endl;
	}
    }

    virtual std::string to_string() const override
    {
      return to_string(fn, text);
    }

    static std::string to_string(const std::string& fn, const std::string& text)
    {
      return "ProcFS: " + fn + " -> " + string::trim_crlf_copy(text);
    }

    static void write_sys(const std::string& fn, const std::string& text, Stop* async_stop=nullptr)
    {
      const unsigned int n_retries = 200;
      const unsigned int milliseconds_per_retry = 100;
      volatile bool stop = false;

      // allow asynchronous stop
      Stop::Scope stop_scope(async_stop, [&stop]() {
	  stop = true;
	});

      for (unsigned int i = 0; i < n_retries && !stop; ++i)
	{
	  if (file_exists(fn))
	    {
	      try {
		SKVPN_LOG("ProcFS: " << fn << " -> '" << string::trim_crlf_copy(text) << '\'');
		write_text_unix(fn, 0777, 0, text);
	      }
	      catch (const std::exception& e)
		{
		  SKVPN_LOG("ProcFS exception: " << e.what());
		}
#ifdef SKVPN_PROCFS_DEBUG
	      sleep_milliseconds(100);
	      std::string text_verify;
	      BufferAllocated buf(256, 0);
	      const int status = read_binary_unix_fast(fn, buf);
	      if (status)
		{
		  text_verify = strerror_str(status);
		}
	      else
		{
		  string::trim_crlf(buf);
		  text_verify = buf_to_string(buf);
		}
	      SKVPN_LOG("WRITE_SYS verify fn=" << fn << " text=" << string::trim_crlf_copy(text) << " verify=" << text_verify);
#endif
	      return;
	    }
	  sleep_milliseconds(milliseconds_per_retry);
	}
      if (stop)
	SKVPN_THROW(procfs_error, "file " << fn << " : aborting write attempt due to stop signal");
      else
	SKVPN_THROW(procfs_error, "file " << fn << " failed to appear within " << (n_retries * milliseconds_per_retry / 1000) << " seconds");
    }

  private:
    std::string fn;
    std::string text;
  };

  class IPv4ReversePathFilter : public ProcFS
  {
  public:
    IPv4ReversePathFilter(const std::string& dev, const unsigned int value)
      : ProcFS(key_fn(dev), skvpn::to_string(value))
    {
      SKVPN_LOG("IPv4ReversePathFilter " << dev << " -> " << value);
    }

    static void write(const std::string& dev, const unsigned int value, Stop* stop=nullptr)
    {
      ProcFS::write_sys(key_fn(dev), skvpn::to_string(value), stop);
    }

  private:
    static std::string key_fn(const std::string& dev)
    {
      return printfmt("/proc/sys/net/ipv4/conf/%s/rp_filter", dev);
    }
  };
}
