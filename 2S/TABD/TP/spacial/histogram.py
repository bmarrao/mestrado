import matplotlib.pyplot as plt 
import psycopg2

from datetime import datetime,timedelta


scale = 1/30000
conn = psycopg2.connect("dbname=postgres user=brenin")
cursor_psql = conn.cursor()
sql = "select initial_ts, final_ts from taxi_services;"
cursor_psql.execute(sql)
results = cursor_psql.fetchall()
weekday = []
weeknd = []

for row in results :
    datetime1 = timestamp_to_datetime(row[0])
    datetime2 = timestamp_to_datetime(row[1])


# Bin edges
bins = [1, 2, 3, 4, 5, 6, 7]

# Frequency counts for each bin
frequency = [1, 1, 3, 3, 4, 2, 1]

# Create histogram
plt.hist(bins[:-1], bins=bins, weights=frequency, color='skyblue', edgecolor='black')

# Add labels and title
plt.xlabel('Value')
plt.ylabel('Frequency')
plt.title('Histogram')

# Show plot
plt.show()


