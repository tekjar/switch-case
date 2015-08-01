import pytoml as toml
import json
import sys

def generate(file):
  '''
    generates json from a toml 
  '''
  input_file = file
  output_file = input_file.replace('.toml', '.json')
  with open(input_file, 'r') as tom, open(output_file, 'w') as son:
      tom_tree = toml.load(tom)
      son.write(json.dumps(tom_tree, indent=4, sort_keys=True))
      
if __name__ == '__main__':
  for arg in sys.argv:
    generate(arg)
