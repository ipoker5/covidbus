package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Tipo_gps {
	
	Integer idtipo_gps;
	Float x;
	Float y;
	Integer idsensor;
	
	public Tipo_gps(@JsonProperty("idtipo_gps")Integer idtipo_gps,@JsonProperty("x")Float x,@JsonProperty("y")Float y,@JsonProperty("idsensor") Integer idsensor) {
		super();
		this.idtipo_gps=idtipo_gps;
		this.x=x;
		this.y=y;
		this.idsensor=idsensor;
	}

	public Integer getIdtipo_gps() {
		return idtipo_gps;
	}

	public void setIdtipo_gps(Integer idtipo_gps) {
		this.idtipo_gps = idtipo_gps;
	}

	public Float getX() {
		return x;
	}

	public void setX(Float x) {
		this.x = x;
	}

	public Float getY() {
		return y;
	}

	public void setY(Float y) {
		this.y = y;
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
		result = prime * result + ((idtipo_gps == null) ? 0 : idtipo_gps.hashCode());
		result = prime * result + ((x == null) ? 0 : x.hashCode());
		result = prime * result + ((y == null) ? 0 : y.hashCode());
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
		Tipo_gps other = (Tipo_gps) obj;
		if (idsensor == null) {
			if (other.idsensor != null)
				return false;
		} else if (!idsensor.equals(other.idsensor))
			return false;
		if (idtipo_gps == null) {
			if (other.idtipo_gps != null)
				return false;
		} else if (!idtipo_gps.equals(other.idtipo_gps))
			return false;
		if (x == null) {
			if (other.x != null)
				return false;
		} else if (!x.equals(other.x))
			return false;
		if (y == null) {
			if (other.y != null)
				return false;
		} else if (!y.equals(other.y))
			return false;
		return true;
	}

	

}
