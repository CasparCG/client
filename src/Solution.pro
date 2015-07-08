TEMPLATE = subdirs

SUBDIRS += \
    Common \
    Atem \
    Caspar \
    TriCaster \
    Panasonic \
    Sony \
    Spyder \
    Web \
    Gpi \
    Osc \
    Repository \
    Core \
    Widgets \
    Shell

Core.depends = Atem Caspar TriCaster Osc Gpi Common
Widgets.depends = Atem Caspar TriCaster Panasonic Sony Spyder Web Osc Repository Gpi Common Core
Shell.depends = Atem Caspar TriCaster Panasonic Sony Spyder Web Osc Repository Gpi Common Core Widgets
