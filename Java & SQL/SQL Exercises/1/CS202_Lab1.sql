#1 SELECT name,useful_votes FROM user WHERE useful_votes in(select max(useful_votes) from user)
#2 SELECT name, full_address, stars FROM business WHERE stars BETWEEN 3.5 and 4.5
#3 SELECT * FROM business WHERE state = 'New York' ORDER BY name
#4 SELECT state,avg(stars) FROM business GROUP BY state, city ORDER BY state,city
#5 SELECT name,city,full_address,stars,review_count FROM business WHERE open = '1' and city = 'Las Vegas' ORDER BY stars DESC LIMIT 5;
#6 SELECT business_id,text,type FROM review WHERE type = "cool" 
#AND business_id in (SELECT id FROM business WHERE full_address like '%Lawn Court%')
#7 SELECT name,full_address,state,stars,open FROM business WHERE stars > 4.5 AND open = TRUE AND state = 'Florida'
#8 SELECT name FROM users WHERE funny_votes < some (SELECT funny_votes FROM users WHERE useful_votes > 5)