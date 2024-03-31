import os
import requests
from time import sleep
from datetime import datetime as date
if __name__ == "__main__":
    urlDict = {'.html': 'https://www.toptal.com/developers/html-minifier/api/raw',
               '.htm': 'https://www.toptal.com/developers/html-minifier/api/raw',
               '.css': 'https://www.toptal.com/developers/cssminifier/raw',
               '.js': 'https://www.toptal.com/developers/javascript-minifier/raw'}

    validFormats = ['.htm', '.html', '.css', '.js']  # add required formats

    trailer = ")=====\";"
    temp = {}
    files = os.listdir()
    # init
    for file in files:
        if os.path.isfile(file):
            if file[file.rindex('.'):]:
                temp[file[: file.rindex(".")]] = ""

    while(1):
        sleep(1)  # Delay
        files = os.listdir()  # Delay get all files in the directory
        for file in files:
            if os.path.isfile(file):
                # check if the file is html, css, or js
                if file[file.rindex('.'):] in validFormats:
                    ext = file[file.rindex('.'):]  # file extension
                    fileName = file[: file.rindex(".")]  # fileName
                    # form variable name
                    header = f"const char _{fileName}[] PROGMEM = R\"=====("
                    filePoint = open(file, 'r')
                    content = filePoint.read()  # get file content
                    filePoint.close()
                    # check if it is same as the previous file
                    if(content == temp[fileName]):
                        continue

                    temp[fileName] = content  # update content
                    data = {'input': content}  # request
                    url = urlDict.get(ext)  # url
                    try:
                        data = requests.post(
                            url, data=data).text  # response
                    except:
                        data = content
                        print(f"{file} minification failed")

                    fileContent = header + "\n" + data + "\n" + trailer  # header file formatting

                    filePoint = open(fileName+".h", "w")
                    filePoint.write(fileContent)
                    filePoint.close()

                    # get the date and format conversion
                    dateNow = date.now().strftime("%H:%M:%S %d-%m-%Y")
                    print(f"{file} changed at {dateNow}")
