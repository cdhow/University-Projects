{-# LANGUAGE ScopedTypeVariables #-}

module Main where
import Control.Monad (replicateM_, replicateM)
import Data.Maybe (catMaybes)
import Data.List (minimumBy, intercalate)
import Data.Ord (comparing)

{-
Recursive nature: DFS
        [] - a b c d
          [a] - b c d
            [a c] - b d
              [a c b] - d    -- d doesn't fit bottom right
                1
          [b] - a c d
            [b c] - a d
              [b c a] - d    - 1
              [b c d] - a
                [b c d a] - [] = 0

-}

readIntsFromLine :: IO [Int]
readIntsFromLine = map read . words <$> getLine

-- [1D representation of Puzzle Piece], (top, left, bottom, right)
data Piece = Piece [String] (Int, Int, Int, Int) deriving (Show, Eq)
type Board = [Piece] -- solved so far

getRightEdge :: Piece -> Int
getRightEdge (Piece _ (_, _, _, r)) = r

getBottomEdge :: Piece -> Int
getBottomEdge (Piece _ (_, _, b, _)) = b

-- Wrapper function for solve'
solve :: Int -> [Piece] -> (Board, Int)
solve dim pieces = solve' dim [] pieces

solve' :: Int -> Board -> [Piece] -> (Board, Int)
solve' _ b [] = (b, 0) -- We have 0 pieces remaining, which means the board is complete.
  -- Board is 0, return the failed board and the number of pieces we failed to place on board,
  -- Otherwise return the best possible unsolved solution (solution with minimum pieces remaining).
solve' dim board pieces
  | length boards == 0 = (board, length pieces)
  | otherwise          = minimumBy (comparing snd) results 
    where
        results = map (\b -> solve' dim b (filter (/= head b) pieces)) boards
        boards = catMaybes . map (tryPiece dim board) $ pieces


-- This function tests whether a piece can fit on the board and returns
-- a new state (board) if it can fit.
tryPiece :: Int -> Board -> Piece -> Maybe Board
tryPiece dim b (p@(Piece rows (top, left, bottom, right))) 
  | doesPieceFit = Just (p:b) -- piece fits, add it to the board
  | otherwise    = Nothing    -- piece does not fit, return nothing
    where
        doesPieceFit = all id [topEdgeFits, leftEdgeFits, bottomEdgeFits, rightEdgeFits]

        -- Check to see if the piece is compatible at the position we are testing
        topEdgeFits     = if isLocOnTopEdge     then top == 0     else top + neighbourTop == 0
        leftEdgeFits    = if isLocOnLeftEdge    then left == 0    else left + neighbourLeft == 0
        bottomEdgeFits  = if isLocOnBottomEdge  then bottom == 0  else True -- assume fitting top-to-bottom, left-to-right
        rightEdgeFits   = if isLocOnRightEdge   then right == 0   else True -- assume fitting top-to-bottom, left-to-right

        -- We cons the piece onto the front of the board as this is more efficient
        -- than adding it to the end.
        -- This is the piece's potential neighbours, we will test to see if it is
        -- compatible with these neighbours
        neighbourTop = getBottomEdge . head . drop (dim-1) $ b
        neighbourLeft = getRightEdge . head $ b

        -- Location of where we are trying to place the piece on the board
        isLocOnTopEdge = loc < dim
        isLocOnLeftEdge = loc `rem` dim == 0
        isLocOnBottomEdge = loc >= dim*3
        isLocOnRightEdge = (loc+1) `rem` dim == 0

        loc = length b

parsePiece :: Int -> IO Piece
parsePiece pieceHeight = do
  rows <- replicateM pieceHeight getLine
  [top, left, bottom, right] <- readIntsFromLine
  return $ Piece rows (top, left, bottom, right)


-- Prints a row of pieces
printPieces :: Board -> Int -> IO ()
printPieces board ph = printPieces' board 0 ph
  where
    printPieces' board pr ph = 
      if pr == ph then return () else do
        putStr $ intercalate "" $ (map (\(Piece rows _) -> rows !! pr) board)
        putStrLn ""
        printPieces' board (pr+1) ph
      
-- Prints a Board by recursively printing each row of pieces
-- top to bottom
printBoard :: Board -> Int -> Int -> IO ()
printBoard board dim ph = 
  if length board == 0 then return () else do
    printPieces (take dim board) ph
    printBoard (drop dim board) dim ph

-- Reads, solves and prints solution of a single puzzle
puzzle :: IO ()
puzzle = do
    [dim, pieceHeight, _pieceWidth] <- readIntsFromLine
    pieces <- replicateM (dim*dim) (parsePiece pieceHeight)
    let (b, piecesLeft) = solve dim pieces
    printBoard (reverse b) dim pieceHeight

main :: IO ()
main = do
  puzzleNum :: Int <- read <$> getLine

  replicateM_ puzzleNum puzzle

