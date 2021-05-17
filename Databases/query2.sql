SELECT recipe_contains_food.recipe_name
FROM food JOIN recipe_contains_food ON food.food_name = recipe_contains_food.food_name
WHERE food.protein > 20 AND food.carbohydrates < 20;