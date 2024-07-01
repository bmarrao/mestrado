import psycopg2
import geopandas as gpd
import matplotlib.pyplot as plt

# Connect to your PostgreSQL database
conn = psycopg2.connect("dbname=postgres user=brenin")


# SQL query to retrieve the simplified and unioned geometry of the municipality of Porto
sql_query = """
    SELECT ST_Union(ST_Simplify(proj_boundary, 0.5)) AS geom 
    FROM cont_aad_caop2018 
    WHERE concelho='PORTO';
"""

# Read the result into a GeoDataFrame
gdf = gpd.GeoDataFrame.from_postgis(sql_query, conn, geom_col='geom')

# Close the database connection
conn.close()

# Plot the GeoDataFrame
fig, ax = plt.subplots()
gdf.plot(ax=ax, color='blue')
ax.set_title('Boundary of Municipality of Porto')
plt.show()