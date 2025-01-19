//    SkVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2012-2020 SkVPN Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

#ifndef SKVPN_TUN_WIN_WFP_H
#define SKVPN_TUN_WIN_WFP_H

#include <ostream>

#include <skvpn/common/rc.hpp>
#include <skvpn/common/wstring.hpp>
#include <skvpn/common/action.hpp>
#include <skvpn/buffer/bufstr.hpp>
#include <skvpn/tun/win/tunutil.hpp>
#include <skvpn/win/winerr.hpp>

#include <fwpmu.h>
#include <fwpmtypes.h>
#include <iphlpapi.h>
#include <wchar.h>

#ifdef __MINGW32__

#include <initguid.h>

// WFP-related defines and GUIDs not in mingw32

#ifndef FWPM_SESSION_FLAG_DYNAMIC
#define FWPM_SESSION_FLAG_DYNAMIC 0x00000001
#endif

// defines below are taken from skvpn2 code (https://github.com/SkVPN/skvpn/blob/master/src/skvpn/block_dns.c)
// which likely borrowed them from Windows SDK header fwpmu.h

/* c38d57d1-05a7-4c33-904f-7fbceee60e82 */
DEFINE_GUID(
  FWPM_LAYER_ALE_AUTH_CONNECT_V4,
  0xc38d57d1,
  0x05a7,
  0x4c33,
  0x90, 0x4f, 0x7f, 0xbc, 0xee, 0xe6, 0x0e, 0x82
);

/* 4a72393b-319f-44bc-84c3-ba54dcb3b6b4 */
DEFINE_GUID(
  FWPM_LAYER_ALE_AUTH_CONNECT_V6,
  0x4a72393b,
  0x319f,
  0x44bc,
  0x84, 0xc3, 0xba, 0x54, 0xdc, 0xb3, 0xb6, 0xb4
);

/* d78e1e87-8644-4ea5-9437-d809ecefc971 */
DEFINE_GUID(
  FWPM_CONDITION_ALE_APP_ID,
  0xd78e1e87,
  0x8644,
  0x4ea5,
  0x94, 0x37, 0xd8, 0x09, 0xec, 0xef, 0xc9, 0x71
);

/* c35a604d-d22b-4e1a-91b4-68f674ee674b */
DEFINE_GUID(
  FWPM_CONDITION_IP_REMOTE_PORT,
  0xc35a604d,
  0xd22b,
  0x4e1a,
  0x91, 0xb4, 0x68, 0xf6, 0x74, 0xee, 0x67, 0x4b
);

/* 4cd62a49-59c3-4969-b7f3-bda5d32890a4 */
DEFINE_GUID(
  FWPM_CONDITION_IP_LOCAL_INTERFACE,
  0x4cd62a49,
  0x59c3,
  0x4969,
  0xb7, 0xf3, 0xbd, 0xa5, 0xd3, 0x28, 0x90, 0xa4
);

/* UUID of WFP sublayer used by all instances of skvpn
 * 2f660d7e-6a37-11e6-a181-001e8c6e04a2 */
DEFINE_GUID(
  SKVPN_BLOCK_OUTSIDE_DNS_SUBLAYER,
  0x2f660d7e,
  0x6a37,
  0x11e6,
  0xa1, 0x81, 0x00, 0x1e, 0x8c, 0x6e, 0x04, 0xa2
);

#endif

namespace skvpn {
  namespace TunWin {

    class WFP : public RC<thread_unsafe_refcount>
    {
    public:
      typedef RCPtr<WFP> Ptr;

      SKVPN_EXCEPTION(wfp_error);

      // Block DNS from all apps except skvpn_app_path and
      // from all interfaces except tap_index.
      // Derived from https://github.com/ValdikSS/skvpn-with-patches/commit/3bd4d503d21aa34636e4f97b3e32ae0acca407f0
      void block_dns(const std::wstring& skvpn_app_path,
		     const NET_IFINDEX tap_index,
		     std::ostream& log)
      {
	// WFP filter/conditions
	FWPM_FILTER0 filter = {0};
	FWPM_FILTER_CONDITION0 condition[2] = {0};
	UINT64 filterid = 0;

	// Get NET_LUID object for adapter
	NET_LUID tap_luid = adapter_index_to_luid(tap_index);

	// Get app ID
	unique_ptr_del<FWP_BYTE_BLOB> skvpn_app_id_blob = get_app_id_blob(skvpn_app_path);

	// Populate packet filter layer information
	{
	  FWPM_SUBLAYER0 subLayer = {0};
	  subLayer.subLayerKey = subLayerGUID;
	  subLayer.displayData.name = const_cast<wchar_t*>(L"SkVPN");
	  subLayer.displayData.description = const_cast<wchar_t*>(L"SkVPN");
	  subLayer.flags = 0;
	  subLayer.weight = 0x100;

	  // Add packet filter to interface
	  const DWORD status = ::FwpmSubLayerAdd0(engineHandle(), &subLayer, NULL);
	  if (status != ERROR_SUCCESS)
	    SKVPN_THROW(wfp_error, "FwpmSubLayerAdd0 failed with status=0x" << std::hex << status);
	}

	// Prepare filter
	filter.subLayerKey = subLayerGUID;
	filter.displayData.name = const_cast<wchar_t*>(L"SkVPN");
	filter.weight.type = FWP_UINT8;
	filter.weight.uint8 = 0xF;
	filter.filterCondition = condition;

	// Filter #1 -- permit IPv4 DNS requests from SkVPN app
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
	filter.action.type = FWP_ACTION_PERMIT;
	filter.numFilterConditions = 2;

	condition[0].fieldKey = FWPM_CONDITION_IP_REMOTE_PORT;
	condition[0].matchType = FWP_MATCH_EQUAL;
	condition[0].conditionValue.type = FWP_UINT16;
	condition[0].conditionValue.uint16 = 53;

	condition[1].fieldKey = FWPM_CONDITION_ALE_APP_ID;
	condition[1].matchType = FWP_MATCH_EQUAL;
	condition[1].conditionValue.type = FWP_BYTE_BLOB_TYPE;
	condition[1].conditionValue.byteBlob = skvpn_app_id_blob.get();

	add_filter(&filter, NULL, &filterid);
	log << "permit IPv4 DNS requests from SkVPN app" << std::endl;

	// Filter #2 -- permit IPv6 DNS requests from SkVPN app
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V6;

	add_filter(&filter, NULL, &filterid);
	log << "permit IPv6 DNS requests from SkVPN app" << std::endl;

	// Filter #3 -- block IPv4 DNS requests from other apps
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
	filter.action.type = FWP_ACTION_BLOCK;
	filter.weight.type = FWP_EMPTY;
	filter.numFilterConditions = 1;

	add_filter(&filter, NULL, &filterid);
	log << "block IPv4 DNS requests from other apps" << std::endl;

	// Filter #4 -- block IPv6 DNS requests from other apps
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V6;

	add_filter(&filter, NULL, &filterid);
	log << "block IPv6 DNS requests from other apps" << std::endl;

	// Filter #5 -- allow IPv4 traffic from TAP
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
	filter.action.type = FWP_ACTION_PERMIT;
	filter.numFilterConditions = 2;

	condition[1].fieldKey = FWPM_CONDITION_IP_LOCAL_INTERFACE;
	condition[1].matchType = FWP_MATCH_EQUAL;
	condition[1].conditionValue.type = FWP_UINT64;
	condition[1].conditionValue.uint64 = &tap_luid.Value;

	add_filter(&filter, NULL, &filterid);
	log << "allow IPv4 traffic from TAP" << std::endl;

	// Filter #6 -- allow IPv6 traffic from TAP
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V6;

	add_filter(&filter, NULL, &filterid);
	log << "allow IPv6 traffic from TAP" << std::endl;
      }

      void reset(std::ostream& log)
      {
	engineHandle.reset(&log);
      }

    private:
      class WFPEngine
      {
      public:
	WFPEngine()
	{
	  FWPM_SESSION0 session = {0};

	  // delete all filters when engine handle is closed
	  session.flags = FWPM_SESSION_FLAG_DYNAMIC;

	  const DWORD status = ::FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, &session, &handle);
	  if (status != ERROR_SUCCESS)
	    SKVPN_THROW(wfp_error, "FwpmEngineOpen0 failed with status=0x" << std::hex << status);
	}

	void reset(std::ostream* log)
	{
	  if (defined())
	    {
	      const DWORD status = ::FwpmEngineClose0(handle);
	      handle = NULL;
	      if (log)
		{
		  if (status != ERROR_SUCCESS)
		    *log << "FwpmEngineClose0 failed, status=" << status << std::endl;
		  else
		    *log << "WFPEngine closed" << std::endl;
		}
	    }
	}

	~WFPEngine()
	{
	  reset(nullptr);
	}

	bool defined() const
	{
	  return Win::Handle::defined(handle);
	}

	HANDLE operator()() const
	{
	  return handle;
	}

      private:
	WFPEngine(const WFPEngine&) = delete;
	WFPEngine& operator=(const WFPEngine&) = delete;

	HANDLE handle = NULL;
      };

      static GUID new_guid()
      {
	UUID ret;
	const RPC_STATUS status = ::UuidCreate(&ret);
	if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY)
	  throw wfp_error("UuidCreate failed");
	return ret;
      }

      static NET_LUID adapter_index_to_luid(const NET_IFINDEX index)
      {
	NET_LUID tap_luid;
	const NETIO_STATUS ret = ::ConvertInterfaceIndexToLuid(index, &tap_luid);
	if (ret != NO_ERROR)
	  throw wfp_error("ConvertInterfaceIndexToLuid failed");
	return tap_luid;
      }

      static unique_ptr_del<FWP_BYTE_BLOB> get_app_id_blob(const std::wstring& app_path)
      {
	FWP_BYTE_BLOB *blob;
	const DWORD status = ::FwpmGetAppIdFromFileName0(app_path.c_str(), &blob);
	if (status != ERROR_SUCCESS)
	  SKVPN_THROW(wfp_error, "FwpmGetAppIdFromFileName0 failed, status=0x" << std::hex << status);
	return unique_ptr_del<FWP_BYTE_BLOB>(blob, [](FWP_BYTE_BLOB* blob) {
	    ::FwpmFreeMemory0((void**)&blob);
	  });
      }

      void add_filter(const FWPM_FILTER0 *filter,
		      PSECURITY_DESCRIPTOR sd,
		      UINT64 *id)
      {
	const DWORD status = ::FwpmFilterAdd0(engineHandle(), filter, sd, id);
	if (status != ERROR_SUCCESS)
	  SKVPN_THROW(wfp_error, "FwpmFilterAdd0 failed, status=0x" << std::hex << status);
      }

      const GUID subLayerGUID{new_guid()};
      WFPEngine engineHandle;
    };

    class WFPContext : public RC<thread_unsafe_refcount>
    {
    public:
      typedef RCPtr<WFPContext> Ptr;

    private:
      friend class ActionWFP;

      void block(const std::wstring& skvpn_app_path,
		 const NET_IFINDEX tap_index,
		 std::ostream& log)
      {
	unblock(log);
	wfp.reset(new WFP());
	wfp->block_dns(skvpn_app_path, tap_index, log);
      }

      void unblock(std::ostream& log)
      {
	if (wfp)
	  {
	    wfp->reset(log);
	    wfp.reset();
	  }
      }

      WFP::Ptr wfp;
    };

    class ActionWFP : public Action
    {
    public:
      ActionWFP(const std::wstring& skvpn_app_path_arg,
		const NET_IFINDEX tap_index_arg,
		const bool enable_arg,
		const WFPContext::Ptr& wfp_arg)
	: skvpn_app_path(skvpn_app_path_arg),
	  tap_index(tap_index_arg),
	  enable(enable_arg),
	  wfp(wfp_arg)
      {
      }

      virtual void execute(std::ostream& log) override
      {
	log << to_string() << std::endl;
	if (enable)
	  wfp->block(skvpn_app_path, tap_index, log);
	else
	  wfp->unblock(log);
      }

      virtual std::string to_string() const override
      {
	return "ActionWFP skvpn_app_path=" + wstring::to_utf8(skvpn_app_path) + " tap_index=" + std::to_string(tap_index) + " enable=" + std::to_string(enable);
      }

    private:
      const std::wstring skvpn_app_path;
      const NET_IFINDEX tap_index;
      const bool enable;
      WFPContext::Ptr wfp;
    };
  }
}

#endif
