from flask import Flask
from flask import request
from flask import render_template
from flask import jsonify
from collections import deque
import time
from datetime import datetime
import pytz  # Import the pytz library

app = Flask(__name__)

# creating a variable to hold the data point values
dataPoints = deque(maxlen=100)

# define the time zone we are using
timezone = pytz.timezone('US/Pacific') 

@app.route("/")
# define the variable which will hold our data values being pulled from the cloud server
# includes the photoresistor values, calculated average light level, and the time
def handle_sensor_data():
    pin36Value = request.args.get("reading36")
    pin37Value = request.args.get("reading37")
    pin38Value = request.args.get("reading38")
    pin39Value = request.args.get("reading39")
    avgLightLvl = request.args.get("averageLightLvl")
    timestamp = request.args.get("timestamp")

    if all([pin36Value, pin37Value, pin38Value, pin39Value, avgLightLvl, timestamp]):
        try:
	# convert to float numbers to allow for accuracy
            pin36Value = float(pin36Value)
            pin37Value = float(pin37Value)
            pin38Value = float(pin38Value)
            pin39Value = float(pin39Value)
            avgLightLvl = float(avgLightLvl)
            timestamp = int(timestamp)  # Convert timestamp to integer
            
            pst_time = datetime.fromtimestamp(timestamp, pytz.utc).astimezone(timezone)
            dataPoints .append({
                'timestamp': timestamp,
                'reading36': pin36Value,
                'reading37': pin37Value,
                'reading38': pin38Value,
                'reading39': pin39Value,
                'averageLightLvl': avgLightLvl,
                'pst_formatted_time': pst_time.strftime('%Y-%m-%d %H:%M:%S') # Store PST formatted time
            })
	# print the values onto the AWS Cloud Server terminal
            print(f"Timestamp (PST): {pst_time.strftime('%Y-%m-%d %H:%M:%S')} \nPin 36’s Light Level: {pin36Value} \nPin 37's Light Level: {pin37Value} \nPin 38's Light Level: {pin38Value} \nPin 39’s Light Level: {pin39Value} \nAverage Light Level: {avgLightLvl}")
            return "Data received successfully"
        except ValueError:
            print("Received invalid data") # checking to see if the data was successfully trasmitted
            return "Received invalid data", 400
    else:
        print("Incomplete data")
        return "Incomplete data", 400

# sending our data to our dashboard.html 
@app.route("/dashboard")
def show_dashboard():
    labels = [datetime.fromtimestamp(item['timestamp'], pytz.utc).astimezone(timezone).strftime('%H:%M:%S') for item in dataPoints ]
    # sending the photoresistor value readings and average light level to dashboard.html
    pin36_data = [item['reading36'] for item in dataPoints ]
    pin37_data = [item['reading37'] for item in dataPoints ]
    pin38_data = [item['reading38'] for item in dataPoints ]
    pin39_data = [item['reading39'] for item in dataPoints ]
    avg_data = [item['averageLightLvl'] for item in dataPoints ]
    return render_template('dashboard.html', labels=labels, pin36_data=pin36_data, pin37_data=pin37_data, pin38_data=pin38_data,
                           pin39_data=pin39_data, avg_data=avg_data)

# ensuring the most updated data is collected
@app.route("/latest_data")
def get_latest_data():
    if dataPoints :
        mostRecent = dataPoints [-1]
        time = datetime.fromtimestamp(mostRecent['timestamp'], pytz.utc).astimezone(timezone)
        mostRecent['formatted_timestamp'] = time.strftime('%H:%M:%S')
        return jsonify(latest_data)
    else:
        return jsonify({})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
