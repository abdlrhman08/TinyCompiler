QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    nonterminalnode.cpp \
    terminalnode.cpp \
    parser.cpp

HEADERS += \
    mainwindow.h \
    nonterminalnode.h \
    terminalnode.h \
    parser.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  desc.md \
  test-tiny.tiny

# Rust integration
# Set the path to the Rust crate (Cargo.toml)
RUST_CRATE_DIR = $$PWD/scanner

# Command to build the Rust crate
rust_build_cmd = cargo build --manifest-path $$RUST_CRATE_DIR/Cargo.toml --release

# Build the Rust crate before building the Qt project
QMAKE_EXTRA_COMPILERS += rust_build
rust_build.target = build-rust
rust_build.commands = $$rust_build_cmd
rust_build.CONFIG += no_link

# Link the resulting Rust library to the Qt project
LIBS += -L$$RUST_CRATE_DIR/target/release -lscanner

# Optional: If you are using a specific Rust toolchain, set it here
# For example, if using nightly:
# QMAKE_ENV += RUSTUP_TOOLCHAIN=nightly
