module("luci.controller.watson_datasender_controller", package.seeall)

function index()
	entry({"admin", "services", "watson_datasender"}, cbi("watson_datasender_model"), _("watson_datasender"),105)
end
