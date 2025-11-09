using JuMP
import HiGHS

c = [
    10 7 8;
    10 11 14;
    9 12 4;
    11 13 9
]

min_limits = [1.1e5, 2.2e5, 3.3e5, 4.4e5]
max_limits = [2.75e5, 5.5e5, 6.6e5]

model = Model(HiGHS.Optimizer)

n_companies = 3
n_airports = 4

@variable(model, x[i in 1:n_airports, j in 1:n_companies] >= 0)

@constraint(model, comp_limit[i in 1:n_companies], sum(x[j,i] for j in 1:n_airports) <= max_limits[i])

@constraint(model, air_limit[i in 1:n_airports],sum(x[i,j] for j in 1:n_companies) >= min_limits[i])

@objective(model, Min, sum(c[i,j]*x[i,j] for i in 1:n_airports, j in 1:n_companies))

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("OPTIMAL")
else
    println("NON OPTIMAL: ", termination_status)
end