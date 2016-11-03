#!/usr/bin/env python3

#XTD:xmarus05


#import pprint
import copy
import sys
tabs = {}
root = None
# 0 bit, 1 int, 2, float, 3 NVARCHAR, 4 NTEXT, 5 INT_LINK
types_txt = {0: 'BIT', 1: 'INT', 2: 'FLOAT', 3: 'NVARCHAR', 4: 'NTEXT', 5: 'INT', 6: 'INT'}


def parse(tree, a):
    #pprint.pprint(tree)
    global root
    root = tree.getroot()
    parse_children(root, a, 0)
    return tabs

def exit_validation():
    print("Validation error", file=sys.stderr)
    sys.exit(91)


def is_valid(tables, tree, a, b, g):
    global root
    global tabs
    tabs = {}
    root = tree.getroot()
    parse_children(root, a, 0)

    if not b:
        process_etc(tabs)

    for tab in tabs:
        if "test_a_prk_" + tab + "_id" in tabs[tab].keys():
            print("Conflit in primary key name and name of attribute", file=sys.stderr)
            sys.exit(90)
        #CHECK IF VALID TABLES
        if tab not in tables.keys():
            exit_validation()

        for col in tabs[tab]:
            if col != "PARENT_TABLE_NAME" and col != "PARENT_TABLE_NAME_LVL":
                if not g:
                    if col + "_id" == "prk_" + tab + "_id":
                        print("Conflit in names of PK and FK columns " + col+"_id", file=sys.stderr)
                        sys.exit(90)
                    if "test_a_"+col + "_id" in tabs[tab].keys():
                        print("Conflit in names of FK and attribute columns " + col+"_id", file=sys.stderr)
                        sys.exit(90)
                if col not in tables[tab].keys() or tables[tab][col]['type'] < tabs[tab][col]['type']:
                    exit_validation()


def parse_children(elem, a, i):
    #pprint.pprint(elem)
    i += 1
    # pole elementov v danej urovni, uchovava pocty vyskytov danych elementov
    counter = {}
    for child in elem:
        #pprint.pprint(child)
        # Vytvaranie tabuliek
        str_child = str(child.tag)
        str_child = str_child.lower()
        str_elem = str(elem.tag)
        str_elem = str_elem.lower()
        if str_child not in tabs.keys():
            tabs[str_child] = {}

        # spracovanie atributov daneho childu
        # ak je zadany -a argument, nepridavame atributy
        if a:
            for attrib in child.keys():
                # attrib = attrib.lower()
                # funkcia na zistenie typu atributu
                type_attrib = get_type_num(child.get(attrib), "a")
                # ak neni, pridaj
                attrib = str(attrib)
                attrib = attrib.lower()
                if "test_a_"+attrib not in tabs[str_child].keys():
                    tabs[str_child]["test_a_"+attrib] = {'type': type_attrib, "count": 1, "attr": attrib}
                # ak je, zisti ci typ je vecsi ...
                else:
                    if tabs[str_child]["test_a_"+attrib]['type'] < type_attrib:
                        tabs[str_child]["test_a_"+attrib]['type'] = type_attrib

        # Vytvor childov v countery
        if str_child not in counter.keys():
            counter[str_child] = 1

        # ak sa elem uz nachadza v countery, incrementuj jeho pocet
        else:
            counter[str_child] += 1

        # pokila element nieje root
        # pridaj parent do tabulky
        # TODO: ZMEN parent na nejake UNIQUE meno
        if 'PARENT_TABLE_NAME' not in tabs[str_child].keys():
            tabs[str_child]['PARENT_TABLE_NAME'] = str_elem
            tabs[str_child]['PARENT_TABLE_NAME_LVL'] = i
        #ak je nadradena tabulka s nizsim levelom tak ju prirad
        elif tabs[str_child]['PARENT_TABLE_NAME_LVL'] < i:
            tabs[str_child]['PARENT_TABLE_NAME'] = str_elem
            tabs[str_child]['PARENT_TABLE_NAME_LVL'] = i
        if elem.tag != root.tag:
            # a child neni v tabulke spracovavaneho elementu, pridaj ho
            if str_child not in tabs[str_elem].keys():
                # Do nadradenej tabulky vytvor count a typ = INT_LINK asi kazdy takyto element bude len FK...
                tabs[str_elem][str_child] = {'count':1, 'type': 5}
            else:
                # ak child uz je v tabulke, zistime kolko rovnakych childov
                # je v danej tabulke
                if tabs[str_elem][str_child]['count'] < counter[str_child]:
                    tabs[str_elem][str_child]['count'] = counter[str_child]
            # Ak je tento element uz len datovy, zisti jeho typ
        if child.text and (not child.text.isspace()):
            # print(str_child, child.text)
            type = get_type_num(child.text, "t")
            if 'value' not in tabs[str_child].keys():
                tabs[str_child]['value'] = {'type': type, "count": 1, "attr": None}
            else:
                if tabs[str_child]['value']['type'] < type :
                    tabs[str_child]['value']['type'] = type
        parse_children(child, a, i)
# END OF parse_children


def get_type_num(data, a_or_t = None):

    if not data or data.isspace() or data.lower() in ("true", "false", "0", "1"): # BIT
        return 0

    try: # INT
        int(data)
        return 1
    except:
        pass

    try: # FLOAT
        float(data)
        return 2
    except:
        pass

    if a_or_t == "a": # NVARCHAR OR NTEXT
        #print("AAAAAA"+data.lower()+"aaaa")
        return 3
    else:
        #print("AAAAAA"+data.lower()+"aaaa")
        return 4
# END OF get_type_num


def process_etc(tables,param_etc=None):
    old_tables = copy.deepcopy(tables)
    for tab in old_tables:
        #use list, to be able modify the dictionary
        #print(tab)
        #pprint.pprint(old_tables[tab])
        for col in old_tables[tab]:
            if (col is 'PARENT_TABLE_NAME') or (col is 'PARENT_TABLE_NAME_LVL') or (old_tables[tab][col]['type'] != 5):
                continue
            #print(tab+"[ "+col+" ]")
            #pprint.pprint(old_tables[tab][col])
            col_val = tables[tab].pop(col)
            if param_etc is not None and col_val['count'] > param_etc:
               #print("VYMEEEEEEN ")
               tables[col][tab] = {'count': 1, 'type': 7}
               if tables[tab]['PARENT_TABLE_NAME_LVL'] == 1:
                tables[tab]['PARENT_TABLE_NAME'] = col
                tables[tab]['PARENT_TABLE_NAME_LVL'] = "0"
            elif col_val['count'] != 1:
                for i in range(1, col_val['count']+1):
                    tables[tab][col+str(i)] = {'count': 1, 'type': 6}
            else:
                tables[tab][col] = {'count': 1, 'type': col_val['type']}


# END OF process_etc