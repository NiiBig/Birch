/**
 * Filter a model.
 *
 * - `--config`: Name of the configuration file, if any.
 *
 * - `--input`: Name of the input file, if any. Alternatively, provide this
 *   as `input` in the configuration file.
 *
 * - `--output`: Name of the output file, if any. Alternatively, provide this
 *   as `output` in the configuration file.
 *
 * - `--model`: Name of the model class, if any. Alternatively, provide this
 *   as `model.class` in the configuration file.
 *
 * - `--seed`: Random number seed. Alternatively, provide this as `seed` in
 *   the configuration file. If not provided, random entropy is used.
 *
 * - `--quiet`: Don't display a progress bar.
 */
program filter(
    input:String?,
    output:String?,
    config:String?,
    model:String?,
    seed:Integer?,
    quiet:Boolean <- false) {
  /* config */
  configBuffer:MemoryBuffer;
  if config? {
    auto reader <- Reader(config!);
    configBuffer <- reader.scan();
    reader.close();
  }

  /* random number generator */
  if seed? {
    global.seed(seed!);
  } else if config? {
    auto buffer <- configBuffer.getInteger("seed");
    if buffer? {
      global.seed(buffer!);
    }
  } else {
    global.seed();
  }

  /* model */
  auto buffer <- configBuffer.getObject("model");
  if !buffer? {
    buffer <- configBuffer.setObject("model");
  }
  if !buffer!.getString("class")? && model? {
    buffer!.setString("class", model!);
  }
  auto archetype <- Model?(make(buffer));
  if !archetype? {
    error("could not create model; the model class should be given as " +
        "model.class in the config file, and should derive from Model.");
  }

  /* filter */
  buffer <- configBuffer.getObject("filter");
  if !buffer? {
    buffer <- configBuffer.setObject("filter");
  }
  if !buffer!.getString("class")? {
    buffer!.setString("class", "ParticleFilter");
  }
  auto filter <- ParticleFilter?(make(buffer));
  if !filter? {
    error("could not create filter; the filter class should be given as " +
        "filter.class in the config file, and should derive from ParticleFilter.");
  }

  /* input */
  auto inputPath <- input;
  if !inputPath? {
    inputPath <-? configBuffer.getString("input");
  }
  if inputPath? && inputPath! != "" {
    auto reader <- Reader(inputPath!);
    auto inputBuffer <- reader.scan();
    reader.close();
    inputBuffer.get(archetype!);
  }

  /* output */
  outputWriter:Writer?;
  outputPath:String? <- output;
  if !outputPath? {
    outputPath <-? configBuffer.getString("output");
  }
  if outputPath? && outputPath! != "" {
    outputWriter <- Writer(outputPath!);
    outputWriter!.startSequence();
  }

  /* progress bar */
  bar:ProgressBar;
  if !quiet {
    bar.update(0.0);
  }

  /* filter */
  filter!.initialize(archetype!);
  for t in 0..filter!.size() {
    if t == 0 {
      filter!.filter();
    } else {
      filter!.filter(t);
    }

    /* output current state */
    buffer:MemoryBuffer;
    if outputWriter? {
      filter!.write(buffer, t);
    }

    /* forecast */
    if filter!.nforecasts > 0 {
      /* clone filter for forecast purposes */
      auto filter' <- clone(filter!);

      /* resample current particles, preserve weights */
      filter'.resample(t);
      auto w' <- filter'.w;

      /* turn off delayed sampling, as analytical conditioning on obervations
       * may occur otherwise, which would be invalid for a forecast */
      filter'.delayed <- false;

      /* with these settings, we just filter ahead as normal */
      auto forecast <- buffer.setArray("forecast");
      for s in (t + 1)..(t + filter'.nforecasts) {
        filter'.forecast(s);
        filter'.reduce();
        if outputWriter? {
          auto state <- forecast.push();
          state.set("sample", filter'.x);
          state.set("lweight", w');
          state.set("lnormalize", filter'.lnormalize);
        }
      }
      collect();
    }
    if outputWriter? {
      outputWriter!.print(buffer);
      outputWriter!.flush();
    }
    if !quiet {
      bar.update((t + 1.0)/(filter!.size() + 1.0));
    }
  }

  /* finalize output */
  if outputWriter? {
    outputWriter!.endSequence();
    outputWriter!.close();
  }
}
