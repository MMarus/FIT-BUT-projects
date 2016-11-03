#!/usr/bin/env python3

#Author: Marek Marusic, xmarus05@stud.fit.vutbr.cz
#Purpose: Projekt do predmetu ISJ c.2 stahovani prispevkov z twitteru
#File: twitter.py
from TwitterSearch import *
import argparse
import sys
import time
import re
import requests
import pprint
import urllib
import os

parser = argparse.ArgumentParser(description='Process some params for forum scraper')

class Once(argparse.Action):
    def __call__(self, parser, namespace, values, option_string = None):
        # print '{n} {v} {o}'.format(n = namespace, v = values, o = option_string)
        if getattr(namespace, self.dest) is not None:
            msg = '{o} can only be specified once'.format(o = option_string)
            raise argparse.ArgumentError(None, msg)
        setattr(namespace, self.dest, values)


parser = argparse.ArgumentParser(description='ISJ projekt download tweets from @ArchUpdates', add_help=False)
parser.add_argument('-h','--help', action='count', default= 0, help='Prints this help')
parser.add_argument('--input', help='Input file/stream, when updating', dest="inputfile", action=Once)
parser.add_argument('--output', help='Output file/stream, if specified, output from update process is stored here', dest="outputfile", action=Once)
parser.add_argument('--output-dir', help='Output file/stream, if specified, output from update process is stored here', dest="outputdir", action=Once)
parser.add_argument('--update', help='Opt to update input file', action='store_true')

#parsuj args
try:
    args = parser.parse_args()
except:
    print("Error wrong arguments", file=sys.stderr)
    sys.exit(1)

#VARIABLES
update = False
InStream = None
last = None
since = 597748274070556672
counter = 0
maxCount = 50

#HELP
if args.help > 0:
    if len(sys.argv) == 2:
        parser.print_help()
        sys.exit(0)
    else:
        parser.print_help()
        print('Error, too many arguments', file=sys.stderr)
        sys.exit(1)

#UPDATE
if args.update:
    update = True
    if args.inputfile is None:
        InStream = "ArchUpdates"

    else:
        InStream = args.inputfile

    #asi nacitat subor do pameti

else:
    if args.inputfile is not None:
        print("Error, input file can be used only with --update option", file=sys.stderr)
        sys.exit(1)

#OUTPUT STREAM
if args.outputfile is not None:
    outStream = args.outputfile
else:
    #outStream = sys.stdout
    outStream = "ArchUpdates"

if args.outputdir is not None:
    if outStream != args.outputdir:
        outputDir = args.outputdir
    else:
        print('Error, dir name and file name is the same', file=sys.stderr)
        sys.exit(1)
else:
    if outStream != "ArchUpdatesURLs":
        outputDir = "ArchUpdatesURLs"
    else:
        outputDir = "ArchUpdatesHTMLs"



#ziskaj cislo posledneho tweetu
# plus otvor stary subor
if update:
    try:
        with open(InStream, 'r') as f:
            first_line = f.readline()
            since = re.search("Last tweet\:\s([0-9]*)", first_line)
            if since is not None:
                since = int(since.group(1))

            print(since)

            old_file = f.read()
            #f.close()
            print(old_file)
    except:
        print('Error, cannot open input file', file=sys.stderr)
        sys.exit(1)

#otvor output file
try:
    file = open(outStream, 'w')
except:
        print('Error, cannot open output file', file=sys.stderr)
        sys.exit(1)

#ziskaj prispevky
try:
    tuo = TwitterUserOrder('ArchUpdates') # create a TwitterUserOrder
    tuo.set_since_id(since)

    # it's about time to create TwitterSearch object again
    ts = TwitterSearch(
        consumer_key='KqfUwQtjyWoZimCdj031xuZZw',
        consumer_secret='d2lZ1N0zuSaLo1xBhTW7PbOBP5d0w7s0RHw3ns6cvy8lioBOc5',
        access_token='2591379482-DJb7CLZOYw63DbHZXVZrAz4HUGVoclM2ICsAYz3',
        access_token_secret='heoarqXe38BRke4UwVNnb0YMy5bhAPyVc6AqTXfQhLsrW'
    )

    tweets = ts.search_tweets_iterable(tuo)
except TwitterSearchException as e: # catch all those ugly errors
    print(e)
    sys.exit(1)


# start asking Twitter about the timeline
for tweet in tweets:
    if last is None:
        last = tweet['id']
        file.write("Last tweet: "+str(last)+" downloaded at: "+time.strftime("%d-%m-%Y-%H:%M:%S")+"\n")

    print('@%s tweeted: %s' % ( tweet['user']['screen_name'], tweet['text']))

    url = tweet['entities']['urls'][0]['expanded_url']
    pprint.pprint(url)
    #url = re.search("(http\:\/\/.*)\s", tweet['text'])
    if url is not None:
        dir = re.search("\/\/(.*)\/", url).group(1)
        name = re.search("(.*)\shttps?\:\/\/", tweet['text']).group(1)
        name = name.replace(' ', '-')
        #print(dir)
        #print(name)
        #print("url: "+url)
        if not os.path.exists(outputDir+"/"+dir+"/"):
            os.makedirs(outputDir+"/"+dir+"/")
        try:
            urllib.request.urlretrieve(url, outputDir+"/"+dir+"/"+name+".html")
        except urllib.error.HTTPError:
            print("Error, during downloadin page: "+url, file=sys.stderr)
        except urllib.error.ConnectionError:
            print("Error, during downloadin page: "+url, file=sys.stderr)

    file.write('@%s tweeted: %s\n' % ( tweet['user']['screen_name'], tweet['text']))
    counter += 1

    if update is False and counter >= maxCount:
        break

if update is True:
    if last is None:
        file.write(first_line)
    file.write(old_file)

#file.close()