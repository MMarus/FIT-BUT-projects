#!/usr/bin/env python3

#Author: Marek Marusic, xmarus05@stud.fit.vutbr.cz
#Purpose: Projekt do predmetu ISJ c.1 stahovani fora (stiahne vsetky prispevky z fora do xml)
#File: forum.py

import requests
from lxml import html, etree
import re
import argparse
import sys


def main(xml_out):
    print("Downloading data")
    global update

    # stiahni url


    url_data = requests.get(root_url)

    # parsuj url do xml
    html_data = html.fromstring(url_data.text)

    #rozbi na divy s tabulkami
    for div in html_data.find_class("panel panel-default"):

        #zober tabulku
        table = div.find('table')
        #vytiahni root forum
        root_forum = get_root_forum(table)

        #ked len updatujeme stare xml
        if update:
            forum = old_root.find('forum[@name="'+root_forum.get('name')+'"]')
            #ak forum este neni, treba ho cele oscrapovat
            if forum is None:
                get_forums(table, root_forum, False)
                old_root.append(root_forum)

            #treba ho updatnut
            else:
                get_forums(table, forum, True)

        else:
            get_forums(table, root_forum, False)
            xml_out.append(root_forum)
        #return
        #ziskaj fora


def get_root_forum(table):
    root_name = table.find('thead/tr/th/a')
    if root_name is not None:
        root_forum = etree.Element("forum", name=root_name.text_content())
        return root_forum


#ak chceme updatovat dany element je update True
def get_forums(table, out, update):

    for tr in table.xpath('tbody/tr'):
        td = tr.find('td[@title]')

        #ak je to element s nazvom fora
        if td is not None:
            #etree.dump(td)
            link = td.find('.//a')
            #print("")
            #etree.dump(link)
            if link is not None:
                #meno fora
                forum_name = etree.Element("forum", name=link.text_content())
                small_list = td.xpath('small')
                #ziskaj popis
                if len(small_list) > 0:
                    forum_name.attrib["description"] = small_list[0].text_content()

                #ziskaj kedy bol naposledy pridany prispevok
                last = tr.find('td[@class="center"]//small')
                if last is not None:
                    last = convert_date(last.text_content())
                    #print(str(test))
                    forum_name.attrib["last"] = last
                    #debug zarazka
                    #updatujeme ?
                    #print(link.get('href'))
                    if update:
                        old_forum = out.find('forum[@name="'+link.text_content()+'"]')

                        if old_forum is not None:
                            #print(old_forum.get('last'))
                            #print(last)
                            #porovname datumy
                            diff_date = date_cmp(old_forum.get('last'), last)
                            #print(diff_date)
                            #ak je stare alebo rovnaky datum, pokracuj
                            if diff_date in [0, 1]:
                                continue
                            #ak je novsie, volaj update
                            else:
                                #print("1 "+link.get('href'))
                                old_forum.attrib["last"] = last
                                get_forum(link.get('href'), old_forum, True)
                        #este take neni
                        else:
                            #print("2 "+link.get('href'))
                            get_forum(link.get('href'), forum_name, False)
                            out.append(forum_name)
                    else:
                        #print("3 "+link.get('href'))
                        get_forum(link.get('href'), forum_name, False)
                        out.append(forum_name)
                #return
                global forums
                forums += 1

    #etree.dump(out)


def get_forum(url, xml_out, update):
    url_data = requests.get(root_url+url)
    html_data = html.fromstring(url_data.text)

    for div in html_data.find_class("panel panel-default"):

        #zisti ci je to div s forami
        img = div.find_class("fa fa-bullhorn")

        if len(img) > 0:
            for table in div.xpath('table'):
                get_forums(table, xml_out, update)

        #div s temami
        img = div.find_class("fa fa-group")
        if len(img) > 0:
            for table in div.xpath('table'):
                get_themes(url, xml_out, update)



def get_themes(url, forum, update):
    #print(url)
    url_data = requests.get(root_url+url)
    html_data = html.fromstring(url_data.text)


    for div in html_data.find_class("panel panel-default"):
        #ak su to temy
        img = div.find_class("fa fa-group")
        if len(img) > 0:
            for tr in div.xpath('table/tbody/tr'):

                #etree.dump(td)
                #break
                #print(td.get('class'))
                #ak je to element s nazvom fora
                link = tr.find('td//a[@class="topictitle"]')

                if link is not None:
                    #etree.dump(link)
                    #ak je tema
                    theme = etree.Element("theme", name=link.text_content())
                    id = re.search("viewtopic\.php\?f\=[0-9]*\&t\=([0-9]*)", link.get('href')).group(1)
                    #print(id)
                    theme.attrib["id"] = id

                    author_e = tr.xpath("td//a[contains(@href,'memberlist')]")
                    if len(author_e) > 0:
                        author_e = author_e[0]
                        theme.attrib["author"] = author_e.text_content()


                    #vyber posledny datum
                    last = tr.find('td[@class="center"]//small')
                    #etree.dump(last)

                    if last is not None:
                        last = convert_date(last.text_content())
                        theme.attrib["last"] = last

                        #print(link.text_content())
                        if update:
                            old_theme = forum.find('.//theme[@id="'+str(id)+'"]')

                            if old_theme is not None:
                                #etree.dump(old_theme)

                                #porovname datumy
                                diff_date = date_cmp(old_theme.get('last'), last)
                                #print(diff_date)

                                #ak je stare alebo rovnaky datum, pokracuj
                                if diff_date in [0, 1]:
                                    continue
                                #ak su novsie prispevky, volaj update
                                else:
                                    #print("1 "+link.get('href'))
                                    old_theme.attrib["last"] = last
                                    get_articles(link.get('href'), old_theme, True)
                            #este taka tema nieje
                            else:
                                #print("2 "+link.get('href'))
                                get_articles(link.get('href'), theme, False)
                                forum.append(theme)

                        else:
                            get_articles(link.get('href'), theme, False)
                            forum.append(theme)
                    #return
                global themes
                themes += 1

            #debug zarazka

            #DALSI (ak sa da ist na dalsiu stranu, znova zavolame s linkom dalsej strany)
            pagination = html_data.find_class("pagination pagination-sm hidden-xs")
            if len(pagination) > 0:
                pagination = pagination[0].xpath('li/a[contains(.,"Další")]')
            if len(pagination) > 0:
                get_themes(pagination[0].get('href'), forum, update)


def get_articles(url, theme, update):
    print(url)
    url_data = requests.get(root_url+url)
    html_data = html.fromstring(url_data.text)

    for item in html_data.findall('.//article'):
        article = etree.Element("article")
        #nazov
        name = item.find('.//h3/a')
        article.attrib['name'] = name.text_content()
        print(name.text_content())

        #cislo prispevku
        #print(name.get('href'))
        id = re.search("\#p([0-9]*)", name.get('href')).group(1)
        article.attrib['id'] = str(id)

        if update:
            old_article = theme.find('.//article[@id="'+str(id)+'"]')
            if old_article is not None:
                #etree.dump(old_article)
                continue


        #kto
        who = item.find('.//div[@class="clearfix"]/.//strong/a')
        if who is None:
            who = item.find('.//div[@class="clearfix"]/.//strong')
        etree.SubElement(article, "who").text = who.text_content()

        #kedy pridal prispevok
        when_div = item.find('.//div[@class="pull-left timepost"]')
        when_txt = html.etree.tostring(when_div, encoding="utf-8")
        when_txt = re.search("fa-clock-o\"\/\>(.*)", when_txt.decode("utf-8")).group(1)
        when_txt = convert_date(when_txt)
        print(when_txt)
        etree.SubElement(article, "when").text = when_txt


        #co je vnutri prispevku = class="contet"
        content = item.find('.//div[@class="content"]')
        #TODO: odkomentuj.
        etree.SubElement(article, "content").text = content.text_content()
        global articles
        articles += 1
        theme.append(article)
        #return

    pagination = html_data.find_class("pagination pagination-sm hidden-xs")
    if len(pagination) > 0:
        pagination = pagination[0].xpath('li/a[contains(.,"Další")]')
    if len(pagination) > 0:
        #print(pagination[0].get('href'))
        get_articles(pagination[0].get('href'), theme, update)


def convert_date(date):
    if date == "":
        day = "01"
        mont = "led"
        year = "1990"
        time = "00:00:00"
    else:
        #search("fa-clock-o\"\/\>(.*)", when_txt.decode("utf-8")).group(1)
        #print(date)
        day = re.search("([0-9][0-9])\.", date).group(1)
        #print("day: "+day)
        month = re.search(".*[0-9][0-9]\.\s(\D{3})", date).group(1)
        #print("month: "+str(months[month]))
        year = re.search("([0-9]{4})", date).group(1)
        #print("year: "+year)
        time = re.search("([0-9]+\:[0-9]{2}\:[0-9]{2})", date).group(1)
        #print("time: "+time)
    return day+"-"+months[month]+"-"+year+"-"+time
# END OF convert_date


def date_cmp(date1, date2):
    if date1 == "" or date2 == "":
        return 2
    #print(date1)
    #print(date2)

    #years
    year1 = int(re.search("\-([0-9]{4})\-", date1).group(1))
    #print("year1: "+str(year1))
    year2 = int(re.search("\-([0-9]{4})\-", date2).group(1))
    #print("year1: "+str(year2))
    if year1 > year2:
        return 1
    elif year1 < year2:
        return 2

    #months
    month1 = int(re.search("\-([0-9]{2})\-", date1).group(1))
    #print("month1: "+str(month1))
    month2 = int(re.search("\-([0-9]{2})\-", date2).group(1))
    #print("month2: "+str(month2))
    if month1 > month2:
        return 1
    elif month1 < month2:
        return 2

    #days
    day1 = int(re.search("([0-9]{2})\-", date1).group(1))
    #print("day1: "+str(day1))
    day2 = int(re.search("([0-9]{2})\-", date2).group(1))
    #print("day2: "+str(day2))
    if day1 > day2:
        return 1
    elif day1 < day2:
        return 2

    #hours
    hour1 = int(re.search("\-([0-9]?[0-9])\:", date1).group(1))
    #print("hour1: "+str(hour1))
    hour2 = int(re.search("\-([0-9]?[0-9])\:", date2).group(1))
    #print("hour2: "+str(hour2))
    if hour1 > hour2:
        return 1
    elif hour1 < hour2:
        return 2

    #minutes
    minutes1 = int(re.search("\:([0-9]{2})\:", date1).group(1))
    #print("minutes1: "+str(minutes1))
    minutes2 = int(re.search("\:([0-9]{2})\:", date2).group(1))
    #print("minutes2: "+str(minutes2))
    if minutes1 > minutes2:
        return 1
    elif minutes1 < minutes2:
        return 2

    #sec
    sec1 = int(re.search("\:([0-9]{2})$", date1).group(1))
    #print("sec1: "+str(sec1))
    sec2 = int(re.search("\:([0-9]{2})$", date2).group(1))
    #print("sec2: "+str(sec2))
    if sec1 > sec2:
        return 1
    elif sec1 < sec2:
        return 2

    return 0
# END OF date_cmp


parser = argparse.ArgumentParser(description='Process some params for forum scraper')

class Once(argparse.Action):
    def __call__(self, parser, namespace, values, option_string = None):
        # print '{n} {v} {o}'.format(n = namespace, v = values, o = option_string)
        if getattr(namespace, self.dest) is not None:
            msg = '{o} can only be specified once'.format(o = option_string)
            raise argparse.ArgumentError(None, msg)
        setattr(namespace, self.dest, values)


parser = argparse.ArgumentParser(description='ISJ projekt download articles from forum', add_help=False)
parser.add_argument('-h','--help', action='count', default= 0, help='Prints this help')
parser.add_argument('--input', help='Input file/stream, when updating', dest="inputfile", action=Once)
parser.add_argument('--output', help='Output file/stream, if specified, output from update process is stored here', dest="outputfile", action=Once)
parser.add_argument('--update', help='Opt to update input file', action='store_true')

#parsuj args
try:
    args = parser.parse_args()
except:
    print("Error wrong arguments", file=sys.stderr)
    sys.exit(1)

months = {'led': "01", 'úno': "02", 'bře': "03", 'dub': "04", 'kvě': "05", 'čer': "06", 'črc': "07", 'srp': "08", 'zář': "09", 'říj': "10", 'lis': "11", 'pro': "12"}

root_url = "http://zabublame.cz/"
#output xml
xml_out = etree.Element("zabublame.cz")
output = xml_out

#VARIABLES
old_root = None
update = False
InStream = None

#stats
forums = 0
articles = 0
themes = 0

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
        InStream = "zabublame.cz.xml"
    else:
        InStream = args.inputfile

    parser = etree.XMLParser(remove_blank_text=True)
    xml_old = etree.parse(InStream, parser)
    old_root = xml_old.getroot()
    output = old_root
else:
    if args.inputfile is not None:
        print("Error, input file can be used only with --update option", file=sys.stderr)
        sys.exit(1)

#OUTPUT STREAM
if args.outputfile is not None:
    outStream = args.outputfile
else:
    #outStream = sys.stdout
    outStream = "zabublame.cz.xml"


main(xml_out)
with open(outStream, 'w') as f: ## Write document to file
    f.write(html.etree.tostring(output, pretty_print=True, encoding="utf-8").decode("utf-8"))


print("forums "+str(forums)+" articles= "+str(articles)+" themes = "+str(themes))












