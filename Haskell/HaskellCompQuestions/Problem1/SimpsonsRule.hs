{- 
formula is 
((b-a)/n)/3 * (f(x0) + 4f(x1) + 2f(x2) + 4f(x3) + 2f(x4) +...+ 4f(xn-1) + f(xn))
where b and a = bounds
      n       = number of rectangles
      f       = line function
 coefficients = [1] : (take (n-1) [4,2,4,2..]) : [1]
      x0...xn = the points for the n segments, so n+1 points from a to b
                therefore we do dist = (b-a)/n and xs = [a, dist*a...b]
-}

simpsons         :: (Double -> Double) -> Double -> Double -> Int -> Double
simpsons f a b n  = sum [w * f x | (x,w) <- zip xs ws] * delta_x / 3.0
    where delta_x = (b-a) / fromIntegral n
          xs      = [a, a+delta_x..b]
          ws      = [1.0] ++ (take (n-1) (cycle [4.0,2.0])) ++ [1.0]
          

f   :: Double -> Double
f x = 1/(x+1)

main :: IO ()                                  
main = do
    putStrLn $ show $ simpsons (f) 1.0 10.0 4