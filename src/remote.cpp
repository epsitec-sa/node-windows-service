#include <node_api.h>

#include <windows.h>
#include <string>
#include <psapi.h>
#include <tchar.h>

#define TERMINAL_SERVER_KEY _T("SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\")
#define GLASS_SESSION_ID _T("GlassSessionId")

// _ -> int
napi_value IsCurrentSessionRemoteable(napi_env env, napi_callback_info info)
{
  napi_status status;
  napi_value napiResult;

  BOOL fIsRemoteable = FALSE;

  if (GetSystemMetrics(SM_REMOTESESSION))
  {
    fIsRemoteable = TRUE;
  }
  else
  {
    HKEY hRegKey = NULL;
    LONG lResult;

    lResult = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        TERMINAL_SERVER_KEY,
        0, // ulOptions
        KEY_READ,
        &hRegKey);

    if (lResult == ERROR_SUCCESS)
    {
      DWORD dwGlassSessionId;
      DWORD cbGlassSessionId = sizeof(dwGlassSessionId);
      DWORD dwType;

      lResult = RegQueryValueEx(
          hRegKey,
          GLASS_SESSION_ID,
          NULL, // lpReserved
          &dwType,
          (BYTE *)&dwGlassSessionId,
          &cbGlassSessionId);

      if (lResult == ERROR_SUCCESS)
      {
        DWORD dwCurrentSessionId;

        if (ProcessIdToSessionId(GetCurrentProcessId(), &dwCurrentSessionId))
        {
          fIsRemoteable = (dwCurrentSessionId != dwGlassSessionId);
        }
      }
    }

    if (hRegKey)
    {
      RegCloseKey(hRegKey);
    }
  }

  int result = fIsRemoteable == TRUE ? 1 : 0;
  status = napi_create_int32(env, result, &napiResult);

  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return napiResult;
}

napi_value Init(napi_env env, napi_value exports)
{
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, IsCurrentSessionRemoteable, NULL, &fn);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to wrap IsCurrentSessionRemoteable native function");
  }

  status = napi_set_named_property(env, exports, "is_current_session_remoteable", fn);
  if (status != napi_ok)
  {
    printf("---%d---", (int)status);
    napi_throw_error(env, NULL, "Unable to populate is_current_session_remoteable exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)