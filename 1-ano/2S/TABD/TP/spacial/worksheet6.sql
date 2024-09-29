select count (*) from taxi_stands,cont_aad_caop2018 where st_within(proj_location, proj_boundary) and freguesia='ramalde';

Select P *100 / T from
    (Select count (*) as P from taxi_services, taxi_stands where st_distance(proj_location,st_transform(initial_point :: geometry,3763)) < 100)As A,
    (Select count (*) as T from taxi_services )As B;

Select  name ,count(*) from
    (Select count (*) as P from taxi_services, taxi_stands where st_distance(proj_location,st_transform(initial_point :: geometry,3763)) < 100) group by name order by 2 desc,

Select o.freguesia , d.freguesia , count (*) 
from cont_aad_caop2018 as o , cont_aad_caop2018 as d, taxi_services 
where st_within(st_transform(final_point :: geometry,3763), d.proj_boundary) 
and st_within(st_transform(initial_point :: geometry,3763), o.proj_boundary)  and o.concelho ='PORTO' and d.concelho='PORTO'
group by 1, 2 ;

select state , SUM(ST_Length(ST_Transform(track, 3763))) from tracks group by state;

select taxi,sum(ST_length(ST_Transform(track, 3763))) from tracks where state='BUSY' group by 1 order by 2 desc limit 10;

select extract(hour from to_timestamp(initial_ts)), count(*) from taxi_services group by 1 order by 1;

select extract(hour from to_timestamp(initial_ts)), count(*) from taxi_services where extract(dow from to_timestamp(initial_ts)) in (0,6) group by 1 order by 1;

select extract(hour from to_timestamp(initial_ts)), count(*) from taxi_services where extract(dow from to_timestamp(initial_ts)) not  in (0,6) group by 1 order by 1;


Select  d.freguesia , count (*) 
from cont_aad_caop2018 as o , cont_aad_caop2018 as d, taxi_services 
where st_within(st_transform(final_point :: geometry,3763), d.proj_boundary) 
and st_within(st_transform(initial_point :: geometry,3763), o.proj_boundary)  and o.concelho ='PORTO' 
group by  1
order by 2 desc ;

select ST_Length(ST_Transform(track, 3763))/1000, extract(day from to_timestamp(ts))
from tracks 
where id = 1000;

