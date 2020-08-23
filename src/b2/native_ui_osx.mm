#include <shared/system.h>
#include <shared/debug.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <string>
#include "native_ui.h"
#include "native_ui_osx.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void MessageBox(const std::string &title,const std::string &text) {
    NSString *nstitle=[[NSString alloc] initWithUTF8String:title.c_str()];

    NSString *nstext=[[NSString alloc] initWithUTF8String:text.c_str()];

    NSAlert *alert=[[NSAlert alloc] init];

    [alert setInformativeText:nstext];

    [alert setMessageText:nstitle];

    [alert setAlertStyle:NSAlertStyleCritical];

    [alert runModal];

    [alert release];
    alert=nil;

    [nstext release];
    nstext=nil;

    [nstitle release];
    nstitle=nil;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void *GetKeyWindow() {
    return [NSApp keyWindow];
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

double GetDoubleClickIntervalSeconds() {
    return [NSEvent doubleClickInterval];
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static void SetDefaultPath(NSSavePanel *panel,const std::string &default_path) {
    if(!default_path.empty()) {
        auto default_url=[NSURL fileURLWithPath:[NSString stringWithUTF8String:default_path.c_str()]];
        [panel setDirectoryURL:[default_url URLByDeletingLastPathComponent]];
        [panel setNameFieldStringValue:default_url.lastPathComponent];
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static NSModalResponse RunModal(NSSavePanel *panel,
                                std::string *result) {
    auto old_key_window=[NSApp keyWindow];

    NSModalResponse response=[panel runModal];
    if(response==NSModalResponseOK) {
        result->assign([[[panel URL] path] UTF8String]);
    } else {
        result->clear();
    }

    /* For some reason, OS X doesn't seem to do this
     * automatically, even though b2 has an app bundle with an
     * Info.plist and whatnot and otherwise seems to behave normally.
     */
    [old_key_window makeKeyWindow];

    return response;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static std::string DoFileDialogOSX(const std::vector<OpenFileDialog::Filter> &filters,
                                   std::vector<OpenFileDialog::Checkbox> *checkboxes,
                                   const std::string &default_path,
                                   NSSavePanel *panel)
{
    SetDefaultPath(panel,default_path);

    if(!filters.empty()) {
        // Special handling for ".*". The OS X dialog doesn't work quite like
        // the Windows one.
        bool all_files=false;
        for(const OpenFileDialog::Filter &filter:filters) {
            for(const std::string &extension:filter.extensions) {
                if(extension==".*") {
                    all_files=true;
                    break;
                }
            }
        }

        if(all_files) {
            // This is the default, so do nothing.
        } else {
            NSMutableArray<NSString *> *types=[NSMutableArray array];

            for(const OpenFileDialog::Filter &filter:filters) {
                for(const std::string &extension:filter.extensions) {
                    ASSERT(!extension.empty());
                    ASSERT(extension[0]=='.');
                    [types addObject:[NSString stringWithUTF8String:extension.substr(1).c_str()]];
                }
            }

            [panel setAllowedFileTypes:types];
        }
    }

    NSMutableArray<NSButton *> *buttons=[NSMutableArray array];

    if(!checkboxes->empty()) {
        for(OpenFileDialog::Checkbox &checkbox:*checkboxes) {
            NSString *title=[NSString stringWithUTF8String:checkbox.caption.c_str()];
            NSButton *button=[NSButton checkboxWithTitle:title
                                                  target:nil
                                                  action:nil];
            [button sizeToFit];
            [buttons addObject:button];
        }

        NSStackView *stackView=[NSStackView stackViewWithViews:buttons];
        [stackView setOrientation:NSUserInterfaceLayoutOrientationVertical];
        [stackView setAlignment:NSLayoutAttributeLeading];
        [panel setAccessoryView:stackView];
    }

    std::string path;
    NSModalResponse response=RunModal(panel,&path);
    if(response==NSModalResponseOK) {
        for(size_t i=0;i<checkboxes->size();++i) {
            NSButton *button=[buttons objectAtIndex:i];
            NSControlStateValue state=[button state];
            printf("%s: %ld\n",[[button title] UTF8String],(long)state);
        }
    }

    return path;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::string OpenFileDialogOSX(const std::vector<OpenFileDialog::Filter> &filters,
                              std::vector<OpenFileDialog::Checkbox> *checkboxes,
                              const std::string &default_path)
{
    auto pool=[[NSAutoreleasePool alloc] init];

    std::string result=DoFileDialogOSX(filters,
                                       checkboxes,
                                       default_path,
                                       [NSOpenPanel openPanel]);

    [pool release];
    pool=nil;

    return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::string SaveFileDialogOSX(const std::vector<OpenFileDialog::Filter> &filters,
                              std::vector<OpenFileDialog::Checkbox> *checkboxes,
                              const std::string &default_path)
{
    auto pool=[[NSAutoreleasePool alloc] init];

    std::string result=DoFileDialogOSX(filters,
                                       checkboxes,
                                       default_path,
                                       [NSSavePanel savePanel]);

    [pool release];
    pool=nil;

    return result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::string SelectFolderDialogOSX(const std::string &default_path) {
    auto pool=[[NSAutoreleasePool alloc] init];

    auto panel=[NSOpenPanel openPanel];

    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:NO];

    SetDefaultPath(panel,default_path);

    std::string result;
    RunModal(panel,&result);

    [pool release];
    pool=nil;

    return result;
}
