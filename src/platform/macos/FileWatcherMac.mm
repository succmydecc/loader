#include <FileWatcher.hpp>

#ifdef GEODE_IS_MACOS

#import <Cocoa/Cocoa.h>
#include <fcntl.h>
#include <iostream>

// static constexpr const auto notifyAttributes = FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE;

FileWatcherMac::FileWatcherMac(ghc::filesystem::path const& file, FileWatchCallback callback, ErrorCallback error) {
	this->m_filemode = ghc::filesystem::is_regular_file(file);

	this->dispatch_source = NULL;
	this->m_file = file;
	this->m_callback = callback;
	this->m_error = error;
	this->watch();
}

FileWatcherMac::~FileWatcherMac() {
	dispatch_source_cancel(reinterpret_cast<dispatch_source_t>(this->dispatch_source));
	this->dispatch_source = NULL;
}

void FileWatcherMac::watch() {
	dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	int fildes = open(this->m_file.string().c_str(), O_EVTONLY);

	__block dispatch_source_t source = dispatch_source_create(DISPATCH_SOURCE_TYPE_VNODE, fildes,
	           DISPATCH_VNODE_DELETE | DISPATCH_VNODE_WRITE | DISPATCH_VNODE_EXTEND | 
	           DISPATCH_VNODE_ATTRIB | DISPATCH_VNODE_LINK | DISPATCH_VNODE_RENAME | 
	           DISPATCH_VNODE_REVOKE, queue);

	this->dispatch_source = reinterpret_cast<void*>(source);

	dispatch_source_set_event_handler(source, ^{
		if (this->m_callback) {
			this->m_callback(this->m_file);
		}
	});
	dispatch_source_set_cancel_handler(source, ^(void) {
		close(fildes);
	});
	dispatch_resume(source);
}

bool FileWatcherMac::watching() const {
	return this->dispatch_source != NULL;
}

#endif
