module("luci.controller.example_controller", package.seeall)

function index()
	entry({"admin", "services", "example_model"}, cbi("example_model"), _("Example Model"),105)
end
