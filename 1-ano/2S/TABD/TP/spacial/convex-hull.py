import matplotlib.pyplot as plt 
import psycopg2

scale = 1/30000
conn = psycopg2.connect("dbname=postgres user=brenin")
cursor_psql = conn.cursor()
sql = "select st_astext(proj_location) from taxi_stands"
cursor_psql.execute(sql)
results = cursor_psql.fetchall()
xs = []
ys = []
for row in results :
    print(row[0])
    point_string = row[0]
    point_string = point_string[9:-2]
    points = point_string.split(',')
    for point in points :

        (x,y) = point.split()
        xs.append(float(x))
        ys.append(float(x))
        print(x,y)

plt.plot(xs, ys , color='red')

plt.show()
