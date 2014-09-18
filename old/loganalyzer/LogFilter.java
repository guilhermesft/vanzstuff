import java.io.IOException;
import java.nio.charset.CharacterCodingException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.collections.IteratorUtils;
import org.apache.commons.collections.ResettableIterator;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.RawComparator;
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


/** Hadoop MapReduce para fazer o filtro do log proposto no exercíco.
 */
public class LogFilter extends Configured implements Tool {

	/* Regex usada para pegar o valor do cookie no registro do log */
	private static final Pattern REGEX_COOKIE = Pattern.compile("\"userid=(?<biscoito>.+)\"");
	/* Regex usada para pegar a data de cada registro de log */
	private static final Pattern REGEX_DATA = Pattern.compile("(?<date>\\d{2}/\\w{3}/\\d{4}:\\d{2}:\\d{2}:\\d{2})");

	/**
	 * Classe responsável pela parte de Map do job
	 */
	public static class LogMapper extends MapReduceBase implements Mapper<LongWritable, Text, Text, Text> {

		/** Método de map que pega a informada como Inputsplit e extrai o userid.
		 * O userid extraido eh a chave da saída do map e o valor eh a própia linha do log
		 * */
		@Override
		public void map(LongWritable key, Text value, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {
			Matcher matcherCookie = REGEX_COOKIE.matcher(value.toString());
			if (matcherCookie.find() ) {
				String cookie = matcherCookie.group("biscoito");
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

		@Override
		public void reduce(Text key, Iterator<Text> values, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {
			// Pega todos os valores e colocar as lista de resultados
			Text t = new Text();
			while (values.hasNext()) {
				byte[] ba = values.next().getBytes();
				t.append(ba, 0, ba.length);
				ba = Text.encode("\n").array();
				t.append(ba, 0, ba.length);
			}
			output.collect(key, t);
		}
	}

	/** Classe utilizada para definir o nome dos arquivos de saída.
	 * O nome de saída deve ser o userid, logo a nome do arquivo eh igual a key
	 */
	public static class MultipleFileOutput extends MultipleTextOutputFormat<Text, Text> {

		@Override
		protected String generateFileNameForKeyValue(Text key, Text value, String name) {
			return key.toString();
		}

	}

	/** Comparator criado para que serja ordernado os registro pela data
	 */
	public static class LogFilterValueComparator implements RawComparator<Text> {

		@Override
		public int compare(Text o1, Text o2) {
			//vamos pegar as datas dos dois registros
			Matcher matcherO1 = REGEX_DATA.matcher(o1.toString());
			Matcher matcherO2 = REGEX_DATA.matcher(o2.toString());
			if (matcherO1.find() && matcherO2.find()) {
				//se conseguiu pegar as duas data, vamos ver quem vem antes de quem
				SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MMM/yyyy:HH:mm:ss");
				try {
					Date d1 = dateFormat.parse(matcherO1.group("data"));
					Date d2 = dateFormat.parse(matcherO2.group("data"));
					return d2.compareTo(d1);
				} catch (ParseException e) {
					e.printStackTrace();
					//deixa como esta
					return 0;
				}
			}
			//well... deixa como estão então
			return 0;
		}

		@Override
		public int compare(byte[] b1, int s1, int l1, byte[] b2, int s2, int l2) {
			try {
				return this.compare(new Text(Text.decode(b1, s1, l1)), new Text(Text.decode(b2, s2, l2)));
			} catch (CharacterCodingException e) {
				return 0;
			}
		}
	}

	@Override
	public int run(String[] args) throws Exception {
		//configura todos os paranaue necessário para o job
		JobConf job = new JobConf(getConf(), LogFilter.class);
		job.setJobName("logfilter");
		for (int i =0 ;i <= args.length-2; i++) {
			FileInputFormat.addInputPath(job, new Path(args[i]));

		}
		FileOutputFormat.setOutputPath(job, new Path(args[args.length-1]));
		job.setMapperClass(LogFilter.LogMapper.class);
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(Text.class);
		job.setCombinerKeyGroupingComparator(LogFilterValueComparator.class);
		job.setReducerClass(LogFilter.LogReducer.class);
		job.setOutputValueClass(Text.class);
		job.setOutputFormat(MultipleFileOutput.class);
		job.setOutputKeyClass(Text.class);
		//roda o job
		JobClient.runJob(job);
		return 0;
	}

	public static void main(String[] args) throws Exception {
		int result = ToolRunner.run(new LogFilter(), args);
		System.exit(result);
	}
}


