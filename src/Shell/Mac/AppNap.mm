#include "AppNap.h"

#include <Foundation/Foundation.h>

AppNap::AppNap()
{
    activity = [[NSProcessInfo processInfo] beginActivityWithOptions:NSActivityLatencyCritical | NSActivityUserInitiated
                                            reason:@"Disable App Nap"];
    [this->activity retain];
}

AppNap::~AppNap()
{
    [[NSProcessInfo processInfo] endActivity:this->activity];
    [this->activity release];
}
