import pytoml as toml
import json

with open('switchboard.toml', 'r') as toml_sb, open('switchboard.json', 'w') as json_sb:
    toml_sb_tree = toml.load(toml_sb)
    json_sb.write(json.dumps(toml_sb_tree, indent=4, sort_keys=True))
