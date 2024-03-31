//#include <windows.h>
//#include <winioctl.h>
//#include <stdio.h>
//
//#define BufferLength 512
//
//void HexOutput(char* buf, size_t len) {
//    printf("引导代码：\n");
//    for (size_t i = 0; i < len; ++i) {
//        if (i > 0 && i % 16 == 0)
//            printf("\n");
//        printf("%02X ", (unsigned char)buf[i]);
//    }
//    printf("\n\n");
//}
//
//BOOL GetDriveGeometry(DISK_GEOMETRY* pdg, const wchar_t* drivePath) {
//    HANDLE hDevice;
//    DWORD junk;
//    char lpBuffer[BufferLength] = { 0 };
//
//    hDevice = CreateFile(drivePath,
//        GENERIC_READ,
//        FILE_SHARE_READ | FILE_SHARE_WRITE,
//        NULL,
//        OPEN_EXISTING,
//        0,
//        NULL);
//
//    if (hDevice == INVALID_HANDLE_VALUE) {
//        printf("Failed to open the drive. Error %ld.\n", GetLastError());
//        return FALSE;
//    }
//
//    BOOL bResult = DeviceIoControl(hDevice,
//        IOCTL_DISK_GET_DRIVE_GEOMETRY,
//        NULL,
//        0,
//        pdg,
//        sizeof(*pdg),
//        &junk,
//        (LPOVERLAPPED)NULL);
//
//    if (!bResult) {
//        printf("DeviceIoControl failed. Error %ld.\n", GetLastError());
//        CloseHandle(hDevice);
//        return FALSE;
//    }
//
//    int blockAddr = (0 * 256 + 0) * 63 + 1 - 1;
//    SetFilePointer(hDevice, (blockAddr * 512), NULL, FILE_BEGIN);
//
//    DWORD dwCB;
//    BOOL bRet = ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);
//    if (!bRet) {
//        printf("ReadFile failed. Error %ld.\n", GetLastError());
//        CloseHandle(hDevice);
//        return FALSE;
//    }
//
//    HexOutput(lpBuffer, 512);
//
//    CloseHandle(hDevice);
//
//    return TRUE;
//}
//
//void GetDriveLayout(const wchar_t* drivePath) {
//    HANDLE hDevice;
//    DWORD junk;
//    DRIVE_LAYOUT_INFORMATION driveLayout;
//    char lpBuffer[BufferLength] = { 0 };
//
//    hDevice = CreateFile(drivePath,
//        GENERIC_READ,
//        FILE_SHARE_READ | FILE_SHARE_WRITE,
//        NULL,
//        OPEN_EXISTING,
//        0,
//        NULL);
//
//    if (hDevice == INVALID_HANDLE_VALUE) {
//        printf("Failed to open the drive. Error %ld.\n", GetLastError());
//        return;
//    }
//
//    BOOL bResult = DeviceIoControl(hDevice,
//        IOCTL_DISK_GET_DRIVE_LAYOUT,
//        NULL,
//        0,
//        &driveLayout,
//        sizeof(driveLayout),
//        &junk,
//        (LPOVERLAPPED)NULL);
//
//    if (!bResult) {
//        printf("DeviceIoControl failed. Error %ld.\n", GetLastError());
//        CloseHandle(hDevice);
//        return;
//    }
//
//    printf("分区个数: %d\n", driveLayout.PartitionCount);
//
//    for (DWORD i = 0; i < driveLayout.PartitionCount; ++i) {
//        PARTITION_INFORMATION partition = driveLayout.PartitionEntry[i];
//        printf("分区 %d:\n", i + 1);
//        printf("类型: %lx\n", partition.PartitionType);
//        printf("偏移: %I64d\n", partition.StartingOffset.QuadPart);
//        printf("大小: %I64d\n", partition.PartitionLength.QuadPart);
//    }
//
//    CloseHandle(hDevice);
//}
//
//int main() {
//    DISK_GEOMETRY pdg;
//    BOOL bResult;
//    ULONGLONG diskSize;
//
//    // U盘的设备路径，可以在设备管理器中查找
//    const wchar_t* usbDrivePath = L"\\\\.\\PhysicalDrive1";
//
//    bResult = GetDriveGeometry(&pdg, usbDrivePath);
//
//    if (bResult) {
//        printf("柱面数 = %I64d\n", pdg.Cylinders.QuadPart);
//        printf("每柱面的磁道数 = %ld\n", (ULONG)pdg.TracksPerCylinder);
//        printf("每磁道扇区数 = %ld\n", (ULONG)pdg.SectorsPerTrack);
//        printf("每扇区的字节数 = %ld\n", (ULONG)pdg.BytesPerSector);
//
//        diskSize = pdg.Cylinders.QuadPart * pdg.TracksPerCylinder *
//            pdg.SectorsPerTrack * pdg.BytesPerSector;
//
//        printf("磁盘大小 = %I64d Bytes = %I64d Gb\n", diskSize,
//            diskSize / (1024 * 1024 * 1024));
//    }
//    else {
//        printf("获取磁盘几何信息失败。\n");
//    }
//
//    GetDriveLayout(usbDrivePath);
//
//    return ((int)bResult);
//}
