package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class DataSensor {
	String timestamp;
	Float valor1;
	Float valor2;
	Integer idsensor;
	
	public DataSensor(@JsonProperty("timestamp")String timestamp, @JsonProperty("valor1")Float valor1,@JsonProperty("valor2")Float valor2,@JsonProperty("idsensor")Integer idsensor) {
		super();
		this.timestamp = timestamp;
		this.valor1 = valor1;
		this.valor2 = valor2;
		this.idsensor = idsensor;
	}

	public String getTimestamp() {
		return timestamp;
	}

	public void setTimestamp(String timestamp) {
		this.timestamp = timestamp;
	}

	public Float getValor1() {
		return valor1;
	}

	public void setValor1(Float valor1) {
		this.valor1 = valor1;
	}

	public Float getValor2() {
		return valor2;
	}

	public void setValor2(Float valor2) {
		this.valor2 = valor2;
	}

	public Integer getIdsensor() {
		return idsensor;
	}

	public void setIdsensor(Integer idsensor) {
		this.idsensor = idsensor;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((idsensor == null) ? 0 : idsensor.hashCode());
		result = prime * result + ((timestamp == null) ? 0 : timestamp.hashCode());
		result = prime * result + ((valor1 == null) ? 0 : valor1.hashCode());
		result = prime * result + ((valor2 == null) ? 0 : valor2.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		DataSensor other = (DataSensor) obj;
		if (idsensor == null) {
			if (other.idsensor != null)
				return false;
		} else if (!idsensor.equals(other.idsensor))
			return false;
		if (timestamp == null) {
			if (other.timestamp != null)
				return false;
		} else if (!timestamp.equals(other.timestamp))
			return false;
		if (valor1 == null) {
			if (other.valor1 != null)
				return false;
		} else if (!valor1.equals(other.valor1))
			return false;
		if (valor2 == null) {
			if (other.valor2 != null)
				return false;
		} else if (!valor2.equals(other.valor2))
			return false;
		return true;
	}

	
	

}
