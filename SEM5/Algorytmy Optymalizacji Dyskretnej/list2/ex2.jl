using JuMP
import HiGHS

n_machines = 3
n_product = 4

# minuty na 1kg wyrobu i - produkt, j -maszyma
time_per_kg = [
    5 10 6;
    3 6 4;
    4 5 3;
    4 2 1
]

max_product_demand = [400, 100, 150, 500]

product_profit = [5, 6, 5, 4]

machine_hour_cost = [2, 2, 3]

model = Model(HiGHS.Optimizer)

@variable(model, p[i in 1:n_product, j in 1:n_machines] >= 0) #w minutach

@constraint(model, max_time[i in 1:n_machines], sum(time_per_kg[j, i]*p[j,i] for j in 1:n_product) <= 3600)

@constraint(model, max_kgs[i in 1:n_product], sum(p[i, j] for j in 1:n_machines) <=max_product_demand[i])

@objective(model, Max, sum(product_profit[i]*sum(p[i, j] for j in 1:n_machines) for i in 1:n_product)-sum(machine_hour_cost[i]*sum(p[j, i] * time_per_kg[j, i] / 60 for j in 1:n_product) for i in 1:n_machines))

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("OPTIMAL")
else
    println("NON OPTIMAL: ", termination_status)
end