#include "FileSystemWatcher.h"

#include <stdexcept>

FileSystemWatcher::FileSystemWatcher(const std::string& absolute_directory) {
	m_change_handle = FindFirstChangeNotification(
		absolute_directory.c_str(),
		true,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE
	);
	if (m_change_handle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Could not create filesystem change handle.");
	}
}

FileSystemWatcher::~FileSystemWatcher() {
	FindCloseChangeNotification(m_change_handle);
}

bool FileSystemWatcher::check_changes(int timeout) {
	DWORD wait_status = WaitForSingleObject(m_change_handle, timeout);

	if (wait_status == WAIT_OBJECT_0) {
		bool success = FindNextChangeNotification(m_change_handle);
		if (!success) {
			throw std::runtime_error("Could not refresh filesystem change handle.");
		}
		return true;
	}
	else {
		return false;
	}

}