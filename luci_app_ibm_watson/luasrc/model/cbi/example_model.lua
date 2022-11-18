map = Map("example", "Example Model")

section = map:section(NamedSection, "example_sct", "example", "Example section")

flag = section:option(Flag, "enable", "Enable", "Enable program")

text = section:option( Value, "text", "Text")

address = section:option(Value, "address", "IP address")
address.datatype = "ip4addr"

return map
