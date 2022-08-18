# модуль графического интерфейса пользователя (GUI)
# приёмник данных из arduino
# построитель графиков

from tkinter import *               # библиотека виджетов GUI
import serial                       # для COM-порта
import matplotlib.pyplot as plt         # для построения графиков
from datetime import datetime as d      # для работы со временем
import time

# обработчики нажатия на кнопки
def getArduinoData(self):
    # принять указанное количество измерений
    datalen = NumberToReceive.get()
    #генерация списков для графиков
    Xg = [0 for i in range(datalen)]
    Tg = [0 for i in range(datalen)]
    Hg = [0 for i in range(datalen)]
    # открыть порт
    ### этот способ открыть приводит к перезагрузке Arduino
    # ser = serial.Serial(ComPortAddress.get(), 9600)
    ### а этот - нет :)
    ser = serial.Serial()
    ser.port = ComPortAddress.get()
    ser.baudrate = 9600
    ser.setDTR(False)       # сигнал, из-за которого перезапуск
    ser.open()
    if(not(ser.is_open)):
        timeLog.insert(END, "Не удалось открыть порт")
        return
    # отправка в Arduino требуемого количества измерений
    if(datalen==10):
        ser.write(b'a')     # a, b, c - чтобы не совпадало
    elif(datalen==20):
        ser.write(b'b')
    else:
        ser.write(b'c')
    # цикл получения данных
    for i in range(datalen):
        # строка символов 8 байт из порта
        s8 = ser.readline()
        # разделение по пробелам
        elems = s8.split()
        # восстановление чисел
        tz = int(elems[0])     # целая часть температуры
        tf = int(elems[1])-1   # дробная часть
        hz = int(elems[2])     # целая часть влажности
        hf = int(elems[3])-1   # дробная часть    
        t = tz + tf/100
        h = hz + hf/100
        Xg[i]=d.now().strftime("%H:%M:%S")
        Tg[i]=t
        Hg[i]=h
        #print("t=", t, " h=", h)
        timeLog.insert(END, Xg[i] + " Температура: "
                            + str(t) + ", влажность: "
                            + str(h)
                       )
        timeLog.yview_moveto(1)     # прокрутка в конец списка
        timeLog.update()
    # end for
    # данные приняты - закрыть порт
    ser.close()
    # построить график при установленном флажке
    if(PlotData.get()):
        fig, ax = plt.subplots()
        ax.plot(Xg, Tg, label="temperature")
        ax.plot(Xg, Hg, label="humidity")
        ax.legend()
        plt.show()
    # end if
# end getArduinoData

def clearLog(self):
    # очистить журнал работы программы
    timeLog.delete(0, timeLog.size()-1)
# end clearLog

root = Tk()
root.title("Arduino+Python Weather Station Project")
root.geometry("560x480")
                                # связанные переменные
ComPortAddress = StringVar()    # обычно на моём компьютере arduino
ComPortAddress.set("COM6")      # определяется на СОМ6
NumberToReceive = IntVar()      # количество измерений
NumberToReceive.set(10)         # предусмотреть значения 10, 20, 50
PlotData = BooleanVar()         # опция строить график
PlotData.set(True)              # по умолчанию - строить

                # виджеты GUI
x = 40
ytop  = 20      # начальная координата
ystep = 40      # шаг по y
y = ytop        # начинаем от верхней границы
                # поле ввода порта
labelPort = Label(root, text = "Порт Arduino")
labelPort.place(x = x, y = y)
y+= ystep
comPortEntry = Entry(root, width = 10, textvariable = ComPortAddress)
comPortEntry.place(x = x, y = y)
y+= ystep
                # радиогруппа выбора количества измерений
labelQty = Label(root, text = "Провести:")
labelQty.place(x = x, y = y)
y+= ystep
qty10 = Radiobutton(root, text = "10 измерений", variable = NumberToReceive, value = 10)
qty10.place(x = x, y = y)
y+= ystep
qty20 = Radiobutton(root, text = "20 измерений", variable = NumberToReceive, value = 20)
qty20.place(x = x, y = y)
y+= ystep
qty50 = Radiobutton(root, text = "50 измерений", variable = NumberToReceive, value = 50)
qty50.place(x = x, y = y)
y+= 1.5*ystep
                # кнопка старта измерений
startButton = Button(root, text = "Пуск измерений", width = 16, bg = "#af0")
startButton.bind("<Button-1>", getArduinoData)
startButton.place(x = x, y = y)
y+= 1.5*ystep
                # опция построения графика
plotOption = Checkbutton(root, text = "Cтроить график", variable = PlotData)
plotOption.place(x = x, y = y)
y+= 2.0*ystep
                # кнопка очистки журнала
clearButton = Button(root, text = "Очистка журнала", width = 16, bg = "#f32")
clearButton.bind("<Button-1>", clearLog)
clearButton.place(x = x, y = y)
                # лог программы с полосой прокрутки
                # создание рамки для вывода лога
frameLog = Frame(root, borderwidth = 1, relief = RIDGE)
frameLog.place(x = 210, y=ytop)
labelLog = Label(frameLog, text = "Журнал работы программы")
labelLog.pack()
                # создание списка для журнала действий и полосы прокрутки
scroller = Scrollbar(frameLog)
scroller.pack(side=RIGHT, fill=Y)
timeLog = Listbox(frameLog, height = 25, width = 45, yscrollcommand = scroller.set)
timeLog.pack()
scroller.config(command = timeLog.yview)
timeLog.insert(END, d.now().strftime("%H:%M:%S") 
                + " Готово работать")

