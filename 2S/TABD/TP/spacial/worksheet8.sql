select taxi_id, date(to_timestamp(initial_ts)), count(*) from taxi_services group by cube(1,2);
select taxi_id, extract(month from to_timestamp(initial_ts)), count(*) from taxi_services group by rollup(1,2) ;
select taxi_id, extract(month from to_timestamp(initial_ts)), count(*) from taxi_services group by rollup(1,2) having taxi_id in (1,2,3) ;
