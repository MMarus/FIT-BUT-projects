#!/usr/bin/env python3

#XTD:xmarus05

#import pprint
import sys
import argparse
import xml.etree.ElementTree as ET
# custom modules
import parsexml
import tabs

parser = argparse.ArgumentParser(description='Process some params for XTD')

class Once(argparse.Action):
    def __call__(self, parser, namespace, values, option_string = None):
        # print '{n} {v} {o}'.format(n = namespace, v = values, o = option_string)
        if getattr(namespace, self.dest) is not None:
            msg = '{o} can only be specified once'.format(o = option_string)
            raise argparse.ArgumentError(None, msg)
        setattr(namespace, self.dest, values)

parser = argparse.ArgumentParser(description='IPP projekt #2', add_help=False)
parser.add_argument('-h','--help', action='count', default= 0, help='Prints this help')
parser.add_argument('--input', help='Input file/stream', dest="inputfile", action=Once)
parser.add_argument('--output', help='Output file/stream', dest="outputfile", action=Once)
parser.add_argument('--header', help='Header', dest="head", action=Once)
parser.add_argument('--etc', help='Max count of columns from same sub elements', type=int)
parser.add_argument('-a', help='Ignore attributes', action="store_false")
parser.add_argument('-b', help='Only one sub element', action="store_true")
parser.add_argument('-g', help='Output just in xml', action="store_true")
parser.add_argument('--isvalid', help='check if is valid')

try:
    args = parser.parse_args()
except:
    print("Error wrong arguments",file=sys.stderr)
    sys.exit(1)

if args.help > 0:
    if len(sys.argv) == 2:
        parser.print_help()
        sys.exit(0)
    else:
        parser.print_help()
        print('Error, too many arguments', file=sys.stderr)
        sys.exit(1)
#pprint.pprint(args.inputfile)

if args.b and args.etc is not None:
    print("Error Parsing arguments", file=sys.stderr)
    sys.exit(1)

if args.inputfile is None:
    InStream = sys.stdin
else:
    InStream = args.inputfile

if args.isvalid is not None and args.g:
    print("Error wrong arguments", file=sys.stderr)
    sys.exit(1)

try:
    parser = ET.XMLParser(encoding="utf-8")
    tree = ET.parse(InStream) # , parser=parser)
except:
    print("Error Parsing stream",file=sys.stderr)
    sys.exit(2)

tables = parsexml.parse(tree, args.a)

# parse etc argument, if not -b set
if not args.b:
    parsexml.process_etc(tables, args.etc)
elif args.etc is not None:
    print("Error Parsing arguments", file=sys.stderr)
    sys.exit(1)

#pprint.pprint(tables)

if args.outputfile is None:
    try:
        OutStream = sys.stdout
    except:
        print("Error Opening Output stream",file=sys.stderr)
        sys.exit(3)
else:
    try:
        OutStream = open(args.outputfile, "w")
    except:
        print("Error Opening Output stream", file=sys.stderr)
        sys.exit(3)

output = ""

if args.g:
    if args.head:
        output = "<!--"+args.head + "-->\n"
    xml = tabs.create_xml(tables)
    if output != "":
        print(output, file=OutStream)
    OutStream.write(xml.decode())
else:
    if args.head:
        output = "--"+args.head + "\n\n"
    if args.isvalid is not None:
        try:
            tree2 = ET.parse(args.isvalid) # , parser=parser)
        except:
            print("Error Parsing in stream",file=sys.stderr)
            sys.exit(2)
        parsexml.is_valid(tables, tree2, args.a, args.b, args.g)
    output += tabs.create_tables(tables)
    print(output, file=OutStream)
OutStream.close()