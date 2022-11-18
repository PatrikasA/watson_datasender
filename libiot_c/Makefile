include $(TOPDIR)/rules.mk

PKG_NAME:=libiot_c
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c.git
PKG_SOURCE_VERSION:=f921dce616af815da2236954c608a2f377c928c5

define  Package/libiot_c
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:= IoT-c library
	DEPENDS:=+libopenssl
endef

define Package/libiot_c/description
	IBM's SDK for communicating with the Watson cloud
endef

define Build/InstalDev
	$(INSTALL_DIR) $(1)/usr/lib/include

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/*.h $(1)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1 $(1)/usr/lib

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1.0 $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1 $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/build/libiotp-as-device.so $(1)/usr/lib
endef

define Package/example-download-git/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1 $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1.0 $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/build/libiotp-as-device.so.1 $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/build/libiotp-as-device.so $(1)/usr/lib
endef

$(eval $(call BuildPackage, libiot_c))