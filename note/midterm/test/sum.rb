# create function
# return is not support...
def sum(n)
    sm = 0
    for i = 1, i <= n, i = i + 1
        sm = sm + i
    end
    sm
end

sum(10)