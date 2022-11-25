map = Map("watson")

section = map:section(NamedSection, "watson_datasender_scp", "watson", "Configuration settings")

flag = section:option(Flag, "enable", "Enable", "Enable program")
orgId = section:option(Value, "orgId", "Organization Id")
typeId = section:option(Value, "typeId", "Type Id")
deviceId = section:option(Value, "deviceId", "Device Id")
token = section:option(Value, "token", "Authentication token")
token.datatype = "string"
return map
