include $(TOPDIR)/rules.mk

PKG_NAME:=watson_datasender
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/watson_datasender
	DEPENDS:=+libubus +libubox +libblobmsg-json +libiot_c_device
	CATEGORY:=Base system
	TITLE:=watson_datasender
endef

define Package/watson_datasender/description
	Connects to the IBM Watson IoT cloud and periodically sends router memory information
endef

define Package/watson_datasender/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/watson_datasender $(1)/usr/bin
	$(INSTALL_BIN) ./files/watson.init $(1)/etc/init.d/watson_datasender
	$(INSTALL_CONF) ./files/watson.config $(1)/etc/config/watson

$(eval $(call BuildPackage,watson_datasender))