/**
 * Bayesian linear regression model with conjugate normal-inverse-gamma
 * prior.
 *
 * The model is given by:
 *   $$\begin{align}
 *   \sigma^2 &\sim \mathrm{Inv\text{-}Gamma}(3, 4/10) \\
 *   \boldsymbol{\beta} &\sim \mathcal{N}(0, I\sigma^2) \\
 *   y_n &\sim \mathcal{N}(\mathbf{x}_n^{\top}\boldsymbol{\beta}, \sigma^2)
 *   \end{align}$$
 * The parameters are the noise variance $\sigma^2$ and vector of
 * coefficients $\boldsymbol{\beta}$. The data consists of observations $y_n$
 * and explanatory variables $\mathbf{x}_n$ for $n=1,\ldots,N$.
 *
 * Run the example using:
 *
 *     birch sample \
 *         --model LinearRegressionModel \
 *         --input-file input/bike_share.json \
 *         --output-file output/bike_share.json \
 *         --nsamples 5
 *
 * The data[^1] is from the Capital Bikeshare system in Washington D.C.
 * for the years 2011 to 2012. The aim is to use weather and holiday
 * information to predict the total number of bike hires on any given
 * day[^2].
 *
 * [^1]: https://archive.ics.uci.edu/ml/datasets/bike+sharing+dataset
 *
 * [^2]: H. Fanaee-T & J. Gama (2014). [Event labeling combining ensemble
 * detectors and background knowledge](http://dx.doi.org/10.1007/s13748-013-0040-3).
 * *Progress in Artificial Intelligence*. **2**:113-127.
 */
class LinearRegressionModel < Model {
  /**
   * Explanatory variables.
   */
  X:Real[_,_];

  /**
   * Regression coefficients.
   */
  β:Random<Real[_]>;

  /**
   * Observation variance.
   */
  σ2:Random<Real>;

  /**
   * Observations.
   */
  y:Random<Real[_]>;
  
  fiber simulate() -> Real {
    N:Integer <- rows(X);
    P:Integer <- columns(X);
    if (N > 0 && P > 0) {
      σ2 ~ InverseGamma(3.0, 0.4);
      β ~ Gaussian(vector(0.0, P), identity(P)*σ2);
      y ~ Gaussian(X*β, σ2);
    }
  }
  
  function input(reader:Reader) {
    X <- reader.getRealMatrix("X")!;
    y <- reader.getRealVector("y")!;
  }
  
  function output(writer:Writer) {
    writer.setRealVector("β", β);
    writer.setReal("σ2", σ2);
  }
}
