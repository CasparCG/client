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
    Shell \
    Schedule

Core.depends = Atem Caspar TriCaster Osc Gpi Common Schedule
Widgets.depends = Atem Caspar TriCaster Panasonic Sony Spyder Web Osc Repository Gpi Common Core Schedule
Shell.depends = Atem Caspar TriCaster Panasonic Sony Spyder Web Osc Repository Gpi Common Core Widgets Schedule
