set sql_safe_updates=0;
update business set review_count = (select count(*) from review where business_id = business.id);
update user set funny_votes = (select count(*) from review where review.type = 'funny' and user_id = user.id);
update user set cool_votes = (select count(*) from review where review.type = 'cool' and user_id = user.id);
update user set useful_votes = (select count(*) from review where review.type = 'useful' and user_id = user.id);