#!/usr/bin/env python3

#XTD:xmarus05

#import pprint
import sys
import xml.etree.ElementTree as ET
from xml.dom import minidom

stop = False

def prettify(elem):
    """Return a pretty-printed XML string for the Element."""
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ", encoding="UTF-8")

types_txt = {0: 'BIT', 1: 'INT', 2: 'FLOAT', 3: 'NVARCHAR', 4: 'NTEXT', 5: 'INT', 6: 'INT', 7: 'INT'}
def create_tables(tables):
    data = ""
    for tab in tables:
        #print(tab)
        data += "CREATE TABLE " + tab + "(\n"


        # kontrola konfliktov v menach
        if "test_a_prk_" + tab + "_id" in tables[tab].keys():
            # vypis chybicku
            print("Conflit in primary key name and name of attribute", file=sys.stderr)
            sys.exit(90)
        # konec kontroly

        data += "\tprk_" + tab + "_id INT PRIMARY KEY,\n"

        for col in tables[tab]:
            if col != "PARENT_TABLE_NAME" and col != "PARENT_TABLE_NAME_LVL":
                name = col
                add = ""
                if tables[tab][col]['type'] in [5, 6, 7]:
                    add = "_id"
                    #print(tables[tab])
                    #print(col+add)
                    # kontrola konfliktov v menach
                    if col + add == "prk_" + tab + "_id":
                        print("Conflit in names of PK and FK columns " + col+add , file=sys.stderr)
                        sys.exit(90)
                    if "test_a_"+col + add in tables[tab].keys():
                        print("Conflit in names of FK and attribute columns " + col+add , file=sys.stderr)
                        sys.exit(90)
                    # konec kontroly
                elif tables[tab][col]['attr'] is not None:
                    #atribut a value su zadane naraz
                    name = tables[tab][col]['attr']
                    if name in tables[tab].keys() and tables[tab][name]['type'] not in [5, 6, 7]:
                        if tables[tab][col]['type'] > tables[tab][name]['type']:
                            tables[tab][name]['type'] = tables[tab][col]['type']
                        continue
                elif "test_a_"+col in tables[tab].keys() and tables[tab][col]['type'] not in [5, 6, 7]:
                    if tables[tab]["test_a_"+col]['type'] > tables[tab][col]['type']:
                            tables[tab][col]['type'] = tables[tab]["test_a_"+col]['type']



                key = tables[tab][col]['type']
                data += "\t" + name + add + " " + types_txt[key] + ",\n"
        data = data[:-2]
        data += "\n);\n\n"
    return data
# END OF print

def create_xml(tables):
    #data = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    #pprint.pprint(tables)
    p = "PARENT_TABLE_NAME"

    #najskor vytvorim tabulecky do xml ETREE
    root_tabs = ET.Element("tables")
    for tab in tables:
        #print(tab)
        table = ET.SubElement(root_tabs, "table", attrib={"name": tab})
        ET.SubElement(table, "relation", attrib={"to": tab, "relation_type": "1:1"})

    #spracujeme relations
    for tab in tables:
        #print(tab)
        # v tab mam teraz spracovavanu tabulku
        #table_current = root_tabs.find("table[@name='"+tab+"']")
        #najdeme element s menom spracovavanej tabulky
        table = root_tabs.find("table[@name='"+tab+"']")
        parent = tables[tab][p]
        #Preskocime tabulky s ROOTom ako parent
        parent_table_xml = root_tabs.find("table[@name='"+parent+"']")


        def get_brother_recursive(table_name):
            global stop
            if table_name == 'PARENT_TABLE_NAME' or table_name == 'PARENT_TABLE_NAME_LVL':
                return

            parent_tab_val = tables[table_name]

            #print("TABLE "+table_name)
            #pprint.pprint( parent_tab_val )
            for rec in parent_tab_val:
                #print("\t"+rec)
                if rec == 'PARENT_TABLE_NAME' or rec == 'PARENT_TABLE_NAME_LVL' or parent_tab_val[rec]['type'] not in [5, 6, 7]:
                    continue
                if parent_tab_val[rec]['type'] == 6:
                    rec = rec[:-1]
                relation = table.find("relation[@to='"+rec+"']")
                if relation is None:
                    ET.SubElement(table, "relation", attrib={"to": rec, "relation_type": "N:M"})
                #print("stop= "+str(stop)+" tabulka = "+rec)
                if parent_tab_val[p] == rec or table_name == rec:
                    if stop:
                        stop = False
                        continue
                    else:
                        stop = True
                get_brother_recursive(rec)






        def get_rel(table_name, rel=None):

            # do tab val si hodime cele pole tabulky
            tab_val = tables[table_name]

            #print(table_name)
            #pprint.pprint(tab_val)


            #najdeme parent element sprac. tab
            parent_table = root_tabs.find("table[@name='"+tab_val[p]+"']")
            #print("\tspracuj "+table_name+ " and parent = "+tab_val[p])
            #Preskocime tabulky s ROOTom ako parent
            if parent_table is None:
                #print(tab_val[p]+" toto asi bude root")
                return
            # rel=(1: "1:1", 2: "1:N",  3: "N:N")
            #print("\t\t"+str(rel))



            #1:1
            if tab_val[p] == tab and (rel is None or rel == 1):
                #print("podmienka1")
                rel = 1
                relation = table.find("relation[@to='"+tab_val[p]+"']")
                if relation is None:
                    relation = ET.SubElement(table, "relation", attrib={"to": tab_val[p], "relation_type": "1:1"})

            #1:N  //// #N:1      treba lepsiu podmienku
            elif tab_val[p] not in tab_val.keys() and (rel is None or rel == 2):
                rel = 2
                #print("podmienka2")
                relation = table.find("relation[@to='"+tab_val[p]+"']")
                if relation is None:
                    relation = ET.SubElement(table, "relation", attrib={"to": tab_val[p], "relation_type": "1:N"})
                else:
                    relation.set("relation_type", "1:N")

                relation = parent_table.find("relation[@to='"+tab+"']")
                if relation is None:
                    relation = ET.SubElement(parent_table, "relation", attrib={"to": tab, "relation_type": "N:1"})
                else:
                    relation.set("relation_type", "N:1")
            elif tab_val[p] in tab_val.keys() and (rel is None or rel == 3):
                if tab_val[tab_val[p]]['type'] == 7:

                    #print(tab_val[p])
                    #print("AAAAAAAAAAAAAAAAa")
                    rel = 3
                    relation = table.find("relation[@to='"+tab_val[p]+"']")
                    if relation is None:
                        relation = ET.SubElement(table, "relation", attrib={"to": tab_val[p], "relation_type": "N:1"})
                    else:
                        relation.set("relation_type", "N:1")

                    relation = parent_table.find("relation[@to='"+tab+"']")
                    if relation is None:
                        relation = ET.SubElement(parent_table, "relation", attrib={"to": tab, "relation_type": "1:N"})
                    else:
                        relation.set("relation_type", "1:N")
            # ak je parent tabulka v klucoch je to N:N
            else:
                #print("podmienka3")
                # aktualna
                rel = 4
                relation = table.find("relation[@to='"+tab_val[p]+"']")
                if relation is None:
                    relation = ET.SubElement(table, "relation", attrib={"to": tab_val[p], "relation_type": "N:M"})

                relation = parent_table.find("relation[@to='"+tab+"']")
                if relation is None:
                    relation = ET.SubElement(parent_table, "relation", attrib={"to": tab, "relation_type": "N:M"})


            get_brother_recursive(tab_val[p])
            #ak je vo vyssej tabulke parent tato tabulka
            if tables[tab_val[p]][p] == table_name:
                return


            get_rel(tab_val[p], rel)
        # END OF TESTIK
        get_rel(tab)





    return prettify(root_tabs)
# END OF create_xml