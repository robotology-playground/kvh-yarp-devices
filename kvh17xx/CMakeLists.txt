# Copyright: (C) 2016 Fondazione Istituto Italiano di Tecnologia
# Authors: Silvio Traversaro
# CopyPolicy: Released under the terms of the GNU LGPL v2.1+

# Compile the plugins
yarp_prepare_device(kvh17xx TYPE yarp::dev::kvh17xx
                            INCLUDE kvh17xx.h
                            DEFAULT ON
                            WRAPPER inertial)


#   yarp_prepare_plugin(<plugin name>
#                       CATEGORY <category>
#                       [PARENT_TYPE <parent class name>]
#                       TYPE <class name>
#                       INCLUDE <header>
#                       [DEFAULT <ON|OFF>]
#                       [OPTION <name>]
#                       [ADVANCED]
#                       [INTERNAL]
#                       [DEPENDS <condition>]
#                       [TEMPLATE <file_name|path_to_a_file>]
#                       [TEMPLATE_DIR <dir>]
#                       [EXTRA_CONFIG <config>]
#                       [CODE <code>]        # Deprecated, used only by carriers
#                       [WRAPPER <wrapper>]  # Deprecated, used only by devices
#
# This macro converts a plugin declaration to code, and to set up a CMake option
# for enabling or disabling the compilation of that plugin.
#
# The ``CATEGORY`` is the type of plugin implemented (for example ``carrier`` or
# ``device``). ``PARENT_TYPE``, if set, is the type corresponding to the
# ``CATEGORY`` type of plugins (for example ``yarp::os::Carrier`` or
# ``yarp::dev::Device``), and it is used in the generation of the code to
# register the plugin with YARP.
# The ``TYPE`` argument is used to specify the class name (including the
# eventual namespace) that implements the plugin.
# ``INCLUDE`` is the header file that should be included and that contains the
# definition of the class.
#
# For example, for a plugin implementing a ``carrier``, the header file
# (``foo/Bar.h``) will contain something like this::
#
#.. code-block:: c++
#
#    namespace foo {
#    class Bar : public yarp::os::Carrier
#    {
#      // Class implementation
#    };
#    }
#
# The corresponding CMake code will be something like::
#
#.. code-block:: cmake
#
#    yarp_prepare_plugin(CATEGORY carrier
#                        TYPE foo::Bar
#                        INCLUDE foo/Bar.h)
#
# If the ``OPTION`` argument is passed, it will be used to name the CMake
# option, otherwise a default name ``ENABLE_<master>_<name>[_<category>]`` will
# be used (``<category>`` is not added if the name already contains it).
# The ``DEFAULT`` argument, if set, will be used to set the default value for
# this option, otherwise the plugin will be disabled by default.
# The ``DOC`` argument can be used to set a description for this option.
# If the``ADVANCED`` option is enabled, this option is marked as advanced in
# CMake.
# If the ``INTERNAL`` option is enabled, this option is marked as internal in
# CMake, and therefore not displayed in CMake gui. This also implies
# `DEFAULT=ON` unless explicitly specified.
#
# The plugin can be conditionally disabled, depending on some conditions (for
# example some option, only on some systems, or only if some library was found)
# by using the ``DEPENDS`` argument. For example ``DEPENDS "FOO;NOT BAR"`` will
# disable it only if at least one the conditions is false, therefore when
# ``FOO`` is false or ``BAR`` is true.
#
# The `TEMPLATE` and `TEMPLATE_DIR` can be used to specify a file name and a
# directory for the template that will be configured and added to the plugin.
# If a template is not specified, a file `yarp_plugin_<CATEGORY>.cpp.in` is
# searched in current directory, in CMake module path, and in YARP cmake modules
# directory. If still it cannot be found, but the `PARENT_TYPE` argument,
# containing the name of the parent class for the plugin, was specified, then a
# default template is generated in current binary directory. Static plugins will
# not work with the default template.
#
# Each variable in the form `KEY=VALUE` passed to the `EXTRA_CONFIG` argument is
# changed to `YARPPLUG_<KEY>` and used when the template is configured. For
# example `EXTRA_CONFIG WRAPPER=foo` generates the `YARPPLUG_WRAPPER` variable
# that is then replaced in the `yarp_plugin_device.

yarp_add_plugin(kvh17xx kvh17xx.cpp kvh17xx.h)

include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kvh17xx ${YARP_LIBRARIES} vt_kvh17xx)  
yarp_install(TARGETS kvh17xx
             COMPONENT runtime
             LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})
yarp_install(FILES kvh17xx.ini  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})


