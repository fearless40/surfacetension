#include <dxgi1_6.h>

namespace DXGI
{
  void GetHardwareAdapter(IDXGIFactory1 *pFactory,
                          IDXGIAdapter1 **ppAdapter,
                          bool requestHighPerformanceAdapter);

  bool CheckTearingSupport();
}
