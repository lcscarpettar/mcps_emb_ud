import requests
import json
import sqlite3
from .SensorIDTimeStamp import SensorIDTimeStamp


class MeasurePublisher:
    def __init__(self):
        self.data = []
        self.server_url = "https://mcpsapi.azurewebsites.net/mcpsapi/Mesure"
        self._createDB()
        self.sensors_id_timestamp = SensorIDTimeStamp()

    def addObjTemp(self, temp):
        sensor_id = self.sensors_id_timestamp.obj_temp_id
        self._addMeasurement(temp, sensor_id)

    def addAmbTemp(self, temp):
        sensor_id = self.sensors_id_timestamp.amb_temp_id
        self._addMeasurement(temp, sensor_id)

    def _addMeasurement(self, value, sensor_id):
        timestamp = self.sensors_id_timestamp.calculateServerTimeStamp()
        m_data = {'Medicion':value, 'IDSensor':sensor_id,
                  'TimeStamp':timestamp}
        self.data.append(m_data)
        try:
            cursor = self.conn.cursor()
            sql_insert = "insert into measures (id ,measure, timestamp) values ({0}, {1}, {2})".format(sensor_id, value, timestamp)
            cursor.execute(sql_insert)
            self.conn.commit()
        except Exception as ex:
            print(ex)

    def _createDB(self):
        self.conn = sqlite3.connect('mcps.db')
        try:
            table_sql = """
CREATE TABLE IF NOT EXISTS measures(
 id integer NOT NULL,
 measure REAL NOT NULL,
 timestamp REAL NOT NULL
);"""
            cursor = self.conn.cursor()
            cursor.execute(table_sql)
        except Exception as ex:
            print(ex)
        try:
            cursor = self.conn.cursor()
            cursor.execute("select id, measure, timestamp from measures")
            rows = cursor.fetchall()
            for row in rows:
                m_data = {'Medicion':float(row[0]), 'IDSensor':int(row[1]),
                  'TimeStamp':float(row[2])}
                self.data.append(m_data)
        except Exception as ex:
            print(ex)

    def publishMeasuares(self):
        if len(self.data) == 0:
            return
        try:
            response = requests.post(self.server_url, json={'Mediciones' : self.data})
            if response.status_code == 200:
                self.data = []
                try:
                    cursor = self.conn.cursor()
                    cursor.execute("delete from measures")
                    self.conn.commit()
                    print("Publicado con exito")
                except Exception as ex:
                    print(ex)
            return response
        except Exception as ex:
            print(ex)
            return None

    
            
        
        
