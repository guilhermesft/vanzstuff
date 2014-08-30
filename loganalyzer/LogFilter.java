import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.MapReduceBase;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reducer;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.lib.MultipleTextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class LogFilter extends Configured implements Tool {

	/**
	 * Classe responsável pela parte de Map do job
	 */
	public static class LogMapper extends MapReduceBase implements Mapper<LongWritable, Text, Text, Text> {

		/* Regex usada para pegar o valor do cookie no registro do log */
		private static final Pattern REGEX_COOKIE = Pattern.compile("\"userid=(?<biscoito>.+)\"");
		
		@Override
		public void map(LongWritable key, Text value, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {
			Matcher matcher = REGEX_COOKIE.matcher(value.toString());
			if (matcher.find()) {
				String cookie = matcher.group("biscoito");
				if (cookie == null || cookie.isEmpty()) {
					return;
				}
				output.collect(new Text(cookie), new Text(value));
			}
		}
	}

	/**
	 * Classe responsável pela parte de reduce do job
	 */
	public static class LogReducer extends MapReduceBase implements Reducer<Text, Text, Text, Text> {

		/* Regex usada para pegar a data de cada registro de log */
		private static final Pattern REGEX_DATA = Pattern.compile("(?<data>\\d{2}/\\w{3}/\\d{4}:\\d{2}:\\d{2}:\\d{2})");
		private List<String> mResult = new LinkedList<String>();

		@Override
		public void reduce(Text key, Iterator<Text> values, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {
			// Pega todos os valores e colocar as lista de resultados
			while (values.hasNext()) {
				mResult.add(values.next().toString());
			}
			// ordena a lista pela data dos log
			Collections.sort(mResult, new Comparator<String>() {
				@Override
				public int compare(String o1, String o2) {
						Matcher matcherO1 = REGEX_DATA.matcher(o1);
						Matcher matcherO2 = REGEX_DATA.matcher(o2);
						if (matcherO1.find() && matcherO2.find()) {
							SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MMM/yyyy:HH:mm:ss");
							Date d1;
							try {
								d1 = dateFormat.parse(matcherO1.group("data"));
								Date d2 = dateFormat.parse(matcherO2.group("data"));
								return d1.compareTo(d2);
							} catch (ParseException e) {
								e.printStackTrace();
								return 0;
							}
						}
					return 0;
				}
			});
			StringBuilder builder = new StringBuilder();
			Iterator<String> it = mResult.iterator();
			while(it.hasNext()){
				builder.append(it.next());
				if(it.hasNext()){
					builder.append(System.lineSeparator());
				}
			}
			output.collect(key, new Text(builder.toString()));
		}
	}

	public static class MultipleFileOutput extends MultipleTextOutputFormat<Text, Text> {

		@Override
		protected String generateFileNameForKeyValue(Text key, Text value, String name) {
			return key.toString();
		}
		
	}
	
	@Override
	public int run(String[] args) throws Exception {
		JobConf job = new JobConf(getConf(), LogFilter.class);
		job.setJobName("logfilter");
		FileInputFormat.setInputPaths(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		job.setMapperClass(LogFilter.LogMapper.class);
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(Text.class);
		job.setReducerClass(LogFilter.LogReducer.class);
		job.setOutputValueClass(Text.class);
		job.setOutputKeyClass(Text.class);
		job.setNumReduceTasks(3);

		job.setOutputFormat(MultipleFileOutput.class);

		JobClient.runJob(job);
		return 0;
	}

	public static void main(String[] args) throws Exception {
		int result = ToolRunner.run(new LogFilter(), args);
		System.exit(result);
	}
}


