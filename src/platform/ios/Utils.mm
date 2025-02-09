#include <helpers/platform.hpp>

#ifdef GEODE_IS_IOS

USE_GEODE_NAMESPACE();

#include <iostream>
#include <sstream>
#include <UIKit/UIKit.h>

bool utils::clipboard::write(std::string const& data) {
	[UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String: data.c_str()];
    return true;
}

std::string utils::clipboard::read() {
	return std::string([[UIPasteboard generalPasteboard].string UTF8String]);
}

#endif
