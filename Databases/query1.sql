

SELECT recipe_contains_food.Recipe_name
FROM (recipe_contains_food)
WHERE recipe_contains_food.Recipe_name

NOT IN 

(SELECT recipe_contains_food.Recipe_name
FROM person_has_illness
	JOIN food_not_allowed_at_illness ON food_not_allowed_at_illness.Illness_name = Person_Has_Illness.Illness_name
    JOIN recipe_contains_food ON recipe_contains_food.Food_name = food_not_allowed_at_illness.Food_name
    Where person_has_illness.PersonID= 5) ;